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

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#include <algorithm>

#define SET_GENERAL "General"
#define SET_APP "Application "

AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
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

QStringList AppSettings::apps() const
{
  QSettings settings;
  QStringList arr = settings.value(SET_GENERAL "/applist").toStringList();
  std::sort(arr.begin(), arr.end(), [&](QString a, QString b){
    return appName(a) + a < appName(b) + b;
  });
  return arr;
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
