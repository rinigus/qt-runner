/****************************************************************************
**
** Copyright (C) 2020 Rinigus https://github.com/rinigus
**
** This file is part of Flatpak Runner.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of the copyright holder nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

#include "appsettings.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QScreen>
#include <QSettings>
#include <QStandardPaths>

#include <algorithm>

#define SET_GENERAL "General"
#define SET_APP "Application "
#define SETTINGS_VERSION 3

// sync with AppSettingsPage.qml
#define THEME_IGNORE  0 // use default
#define THEME_AUTO    1
#define THEME_LIGHT   2
#define THEME_DARK    3
#define THEME_MANUAL -1
#define THEME_MIN_VALUE THEME_MANUAL
#define THEME_MAX_VALUE THEME_DARK

AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
  bool setdefaults = false;
  QSettings settings;

  setdefaults = (setdefaults || settings.value(SET_GENERAL "/version", 0).toInt() < SETTINGS_VERSION);
  setdefaults = (setdefaults || settings.value(SET_APP + defaultApp() + "/env").toMap().size() == 0);

  if (setdefaults)
    {
      QMap<QString, QString> env;
      env["QT_QUICK_CONTROLS_STYLE"] = "Plasma";
      env["QT_QUICK_CONTROLS_MOBILE"] = "1";
      // Recommended in https://bugreports.qt.io/browse/QTBUG-80665
      env["QTWEBENGINE_DISABLE_GPU_THREAD"] = "1";
      // Flags are based on workaround for SFOS (disable-gpu-compositing) and default Android settings:
      //   Multiple Raster Threads: Disabled
      //   Flags enabled by Qt if enable-embedded-switches is given
      env["QTWEBENGINE_CHROMIUM_FLAGS"] = "--disable-gpu-compositing --num-raster-threads=1 --enable-viewport --disable-composited-antialiasing";
      setAppEnv(defaultApp(), env);
    }

  settings.setValue(SET_GENERAL "/version", SETTINGS_VERSION);
}

int AppSettings::appDpi(QString flatpak, bool merged) const
{
  QSettings settings;
  int d = settings.value(SET_APP + flatpak + "/dpi", 0).toInt();
  if (d > 0 || !merged) return d;
  int s = appScaling(flatpak);
  if (s > 1) return (int)(defaultDpi() / ((float)s));
  d = settings.value(SET_APP + defaultApp() + "/dpi", 0).toInt();
  if (d > 0) return d;
  return defaultDpi();
}

QMap<QString, QString> AppSettings::appEnv(QString flatpak, bool merged) const
{
  QSettings settings;
  QMap<QString, QVariant> mv = settings.value(SET_APP + flatpak + "/env").toMap();
  QMap<QString, QString> m;
  for (auto i = mv.constBegin(); i != mv.constEnd(); ++i)
    m.insert(i.key(), i.value().toString());
  if (flatpak == defaultApp()) return m;
  if (merged)
    {
      QMap<QString, QString> d = appEnv(defaultApp());
      for (auto i = d.constBegin(); i != d.constEnd(); ++i)
        if (!m.contains(i.key()))
          m.insert(i.key(), i.value());
    }
  return m;
}

QString AppSettings::appEnvJson(QString flatpak, bool merged) const
{
  QMap<QString, QString> m = appEnv(flatpak, merged);
  QMap<QString, QVariant> v;
  for (auto i = m.constBegin(); i != m.constEnd(); ++i)
    v.insert(i.key(), i.value());
  return QJsonDocument::fromVariant(v).toJson();
}

int AppSettings::appFollowKeyboard(QString flatpak, bool merged) const
{
  QSettings settings;
  int d = settings.value(SET_APP + flatpak + "/followKeyboard", -1).toInt();
  if (d >= 0 || (!merged && flatpak != defaultApp())) return d;
  return settings.value(SET_APP + defaultApp() + "/followKeyboard", 0).toInt();
}

QString AppSettings::appIcon(QString flatpak) const
{
  QSettings settings;
  return settings.value(SET_APP + flatpak + "/icon").toString();
}

QString AppSettings::appName(QString flatpak) const
{
  QSettings settings;
  return settings.value(SET_APP + flatpak + "/name").toString();
}

int AppSettings::appScaling(QString flatpak, bool merged) const
{
  QSettings settings;
  int s = settings.value(SET_APP + flatpak + "/scaling", 0).toInt();
  if (s > 0 || !merged) return s;
  s = settings.value(SET_APP + defaultApp() + "/scaling", 1).toInt();
  if (s < 1) s = 1;
  return s;
}

int AppSettings::appTheme(QString flatpak, bool merged) const
{
  QSettings settings;
  int s = settings.value(SET_APP + flatpak + "/theme", THEME_IGNORE).toInt();
  if (s != THEME_IGNORE || (!merged && flatpak != defaultApp())) return s;
  s = settings.value(SET_APP + defaultApp() + "/theme", THEME_AUTO).toInt();
  return s;
}

QStringList AppSettings::apps() const
{
  QSettings settings;
  QStringList arr = settings.value(SET_GENERAL "/applist").toStringList();
  std::sort(arr.begin(), arr.end(), [&](QString a, QString b){
    return appName(a) + a < appName(b) + b;
  });
  return arr;
}

QString AppSettings::defaultApp() const
{
  return "default";
}

int AppSettings::defaultDpi() const
{
  return qGuiApp->primaryScreen()->physicalDotsPerInch();
}

void AppSettings::updateApps(QString appsInJson)
{
  QJsonArray arr = QJsonDocument::fromJson(appsInJson.toUtf8()).array();
  QSettings settings;
  QStringList applist;
  for (QJsonArray::const_iterator iter=arr.constBegin(); iter!=arr.constEnd(); ++iter)
    {
      QJsonObject obj = (*iter).toObject();
      QString flatpak = obj.value("flatpak").toString();
      QString name = obj.value("name").toString();
      QString icon = obj.value("icon").toString();
      if (flatpak.isEmpty()) continue;
      applist.append(flatpak);
      settings.setValue(SET_APP + flatpak + "/name", name);
      settings.setValue(SET_APP + flatpak + "/icon", icon);
    }
  settings.setValue(SET_GENERAL "/applist", applist);

  emit appListChanged();
}

void AppSettings::rmAppEnvVar(QString flatpak, QString name)
{
  QMap<QString,QString> e = appEnv(flatpak);
  if (e.contains(name))
    {
      e.remove(name);
      setAppEnv(flatpak, e);
    }
}

void AppSettings::setAppDpi(QString flatpak, int dpi)
{
  QSettings settings;
  if (dpi < 1)
    {
      settings.remove(SET_APP + flatpak + "/dpi");
    }
  settings.setValue(SET_APP + flatpak + "/dpi", dpi);
}

void AppSettings::setAppEnv(QString flatpak, QMap<QString, QString> env)
{
  QSettings settings;
  QMap<QString, QVariant> m;
  for (auto i = env.constBegin(); i != env.constEnd(); ++i)
    m.insert(i.key(), i.value());
  settings.setValue(SET_APP + flatpak + "/env", m);
}

void AppSettings::setAppEnvVar(QString flatpak, QString name, QString value)
{
  QMap<QString,QString> e = appEnv(flatpak);
  e.insert(name, value);
  setAppEnv(flatpak, e);
}

void AppSettings::setAppFollowKeyboard(QString flatpak, int follow)
{
  QSettings settings;
  settings.setValue(SET_APP + flatpak + "/followKeyboard", follow);
}

void AppSettings::setAppScaling(QString flatpak, int scaling)
{
  QSettings settings;
  if (scaling < 1)
    {
      settings.remove(SET_APP + flatpak + "/scaling");
    }
  settings.setValue(SET_APP + flatpak + "/scaling", scaling);
}

void AppSettings::setAppTheme(QString flatpak, int theme)
{
  if (theme >= THEME_MIN_VALUE && theme <= THEME_MAX_VALUE)
    {
      QSettings settings;
      settings.setValue(SET_APP + flatpak + "/theme", theme);
    }
}

void AppSettings::setDark(bool dark)
{
  if (dark == m_dark) return;
  m_dark = dark;
  emit darkChanged();
}

void AppSettings::applyTheme(QString flatpak)
{
  if (flatpak.isEmpty()) return;

  int theme = appTheme(flatpak, true);
  if (theme == THEME_MANUAL) return;
  if (theme == THEME_AUTO) theme = m_dark ? THEME_DARK : THEME_LIGHT;

  if (theme != THEME_DARK && theme != THEME_LIGHT)
    {
      qWarning() << "Unsupported theme ID" << theme;
      return;
    }

  QDir dir = QDir::home();
  QString path = QStringLiteral(".var/app/%1/config").arg(flatpak);
  dir.mkpath(path);
  dir.cd(path);
  QFile file(dir.absoluteFilePath("kdeglobals"));
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      qWarning() << "Failed to open file:" << file.fileName();
      return;
    }

  QTextStream txt(&file);
  if (theme == THEME_DARK) txt << QLatin1String("[Theme]\nname=breeze-dark\n");
  if (theme == THEME_LIGHT) txt << QLatin1String("[Theme]\nname=default\n");
}
