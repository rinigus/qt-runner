/****************************************************************************
**
** Copyright (C) 2020 Rinigus https://github.com/rinigus
**
** This file is part of Qt Runner.
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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QStringList>
#include <QMap>

class AppSettings : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool dark READ dark WRITE setDark NOTIFY darkChanged)

public:
  explicit AppSettings(QObject *parent = nullptr);

  Q_INVOKABLE int appDpi(QString program, bool merged=false) const;
  Q_INVOKABLE QMap<QString, QString> appEnv(QString program, bool merged=false) const;
  Q_INVOKABLE QString appEnvJson(QString program, bool merged=false) const;
  Q_INVOKABLE int appFollowKeyboard(QString program, bool merged=false) const;
  Q_INVOKABLE QString appIcon(QString program) const;
  Q_INVOKABLE QString appName(QString program) const;
  Q_INVOKABLE int appScaling(QString program, bool merged=false) const;
  Q_INVOKABLE int appTheme(QString program, bool merged=false) const;

  Q_INVOKABLE QStringList apps() const;

  Q_INVOKABLE QString defaultApp() const;
  Q_INVOKABLE int defaultDpi() const;

  Q_INVOKABLE void updateApps(QString appsInJson);

  Q_INVOKABLE void rmAppEnvVar(QString program, QString name);
  Q_INVOKABLE void setAppDpi(QString program, int dpi);
  Q_INVOKABLE void setAppEnv(QString program, QMap<QString, QString> env);
  Q_INVOKABLE void setAppEnvVar(QString program, QString name, QString value);
  Q_INVOKABLE void setAppFollowKeyboard(QString program, int follow);
  Q_INVOKABLE void setAppScaling(QString program, int scaling);
  Q_INVOKABLE void setAppTheme(QString program, int theme);

  Q_INVOKABLE void applyTheme(QString program);

  bool dark() const { return m_dark; }
  void setDark(bool dark);

signals:
  void appListChanged();
  void darkChanged();

protected:
  bool m_dark = true;
};

#endif // APPSETTINGS_H
