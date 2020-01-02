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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
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

#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include <QProcessEnvironment>

#include <iostream>

#include "runner.h"

#include <QDebug>


Runner::Runner(QString program, QStringList options, QString wayland_socket)
{
  if (program.isEmpty())
    return; // nothing to do

  QString fc = "flatpak";
  QStringList fo;

  QSettings settings;

  // run command
  fo << "run";

  // Wayland
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("WAYLAND_DISPLAY", wayland_socket);

  // dpi
  double dpi = qGuiApp->primaryScreen()->physicalDotsPerInch();
  std::cout << "Screen DPI: " << dpi << "\n";

  double dpiSet = settings.value("main/dpi", -1).toDouble();
  if (dpiSet > 0)
    {
      dpi = dpiSet;
      std::cout << "Screen DPI overwritten via settings: " << dpi << "\n";
    }

  fo << QString("--env=QT_WAYLAND_FORCE_DPI=%1").arg(int(dpi));

  // filesystems
  fo << "--filesystem=/system:ro" << "--filesystem=/vendor:ro";

  // devices
  fo << "--device=all";

  // libhybris
  fo << "--env=HYBRIS_EGLPLATFORM_DIR=/var/run/host/usr/lib/libhybris"
     << "--env=HYBRIS_LINKER_DIR=/usr/lib/arm-linux-gnueabihf/GL/default/lib/libhybris/linker";

  // add program and its options
  fo << program << options;

  // qDebug() << fo;

  // start application
  m_process = new QProcess(this);
  m_process->setProcessEnvironment(env);

  connect(m_process, &QProcess::errorOccurred, this, &Runner::onError);
  connect(m_process, &QProcess::readyReadStandardError, this, &Runner::onStdErr);
  connect(m_process, &QProcess::readyReadStandardOutput, this, &Runner::onStdOut);
  connect(m_process, SIGNAL(finished()), this, SLOT(onFinished()));

  m_command = fc;
  m_options = fo;
  m_flatpak_program = program;
}

void Runner::start()
{
  if (!m_process) return;
  m_process->start(m_command, m_options);
}

void Runner::onError(QProcess::ProcessError /*error*/)
{
  std::cerr << m_process->errorString().toStdString() << "\n";
}

void Runner::onFinished()
{
  onStdErr();
  onStdOut();
  exit();
}

void Runner::onStdErr()
{
  std::cerr << m_process->readAllStandardError().constData() << std::flush;
}

void Runner::onStdOut()
{
  std::cout << m_process->readAllStandardOutput().constData() << std::flush;
}
