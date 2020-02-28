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

#include <QSettings>
#include <QProcessEnvironment>

#include <iostream>

#include "runner.h"

#include <QDebug>


Runner::Runner(QString program, QStringList flatpak_options, QStringList program_options,
               QString wayland_socket, QString dbusaddress, AppSettings &appsettings):
  m_crashed(false), m_exitCode(0)
{
  if (program.isEmpty())
    return; // nothing to do

  QString fc = "flatpak";
  QStringList fo;

  QSettings qset;

  // run command
  fo << "run";

  // Wayland
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("WAYLAND_DISPLAY", wayland_socket);
  env.insert("FLATPAK_MALIIT_CONTAINER_DBUS", dbusaddress);
  if (!env.contains("FLATPAK_GL_DRIVERS"))
    env.insert("FLATPAK_GL_DRIVERS", "host");

  // drop QMLSCENE_DEVICE="customcontext" as we don't have it in environment
  if (env.value("QMLSCENE_DEVICE") == "customcontext")
    env.remove("QMLSCENE_DEVICE");

  // dpi and scaling factor
  fo << QString("--env=QT_WAYLAND_FORCE_DPI=%1").arg(appsettings.appDpi(program, true));
  {
    int scale = appsettings.appScaling(program, true);
    if (scale > 1)
      fo << QString("--env=QT_SCALE_FACTOR=%1").arg(scale);
  }

  // filesystems
  fo << "--filesystem=/system:ro"
     << "--filesystem=/vendor:ro"
     << "--filesystem=/odm:ro";

  // devices
  fo << "--device=all";

  // keyboard
  fo << "--talk-name=org.maliit.server";

  // Set environment variables
  QMap<QString,QString> fpkenv = appsettings.appEnv(program, true);
  for (auto i=fpkenv.constBegin(); i!=fpkenv.constEnd(); ++i)
    fo << "--env=" + i.key() + "=" + i.value();

//  // libhybris
//  fo << "--env=HYBRIS_EGLPLATFORM_DIR=/var/run/host/usr/lib/libhybris"
//     << "--env=HYBRIS_LINKER_DIR=/var/run/host/usr/lib/libhybris/linker";

//  // check if we have HYBRIS_LD_LIBRARY_PATH defined
//  QStringList ldlibs = env.value("HYBRIS_LD_LIBRARY_PATH",
//                                 "/usr/libexec/droid-hybris/system/lib:/vendor/lib:/system/lib").split(':');
//  QStringList ldlibs_processed;
//  for (QString &s: ldlibs)
//    {
//      if (s.startsWith("/usr"))
//        ldlibs_processed.append("/var/run/host" + s);
//      else
//        ldlibs_processed.append(s);
//    }
//  fo << "--env=HYBRIS_LD_LIBRARY_PATH=" + ldlibs_processed.join(':');

//  // set LD_LIBRARY_PATH to the used extension
//  if (qset.value("main/set_ld_path", 1).toInt() > 0)
//    fo << "--env=LD_LIBRARY_PATH=" + qset.value("main/ld_library_path", "/usr/lib/arm-linux-gnueabihf/GL/host/lib").toString();

  // libhybris
  fo << "--env=HYBRIS_EGLPLATFORM_DIR=/usr/lib/arm-linux-gnueabihf/GL/host/lib/libhybris"
     << "--env=HYBRIS_LINKER_DIR=/usr/lib/arm-linux-gnueabihf/GL/host/lib/libhybris/linker";

  // check if we have HYBRIS_LD_LIBRARY_PATH defined
  QStringList ldlibs = env.value("HYBRIS_LD_LIBRARY_PATH",
                                 "/usr/libexec/droid-hybris/system/lib:/vendor/lib:/system/lib").split(':');
  QStringList ldlibs_processed;
  ldlibs_processed << "/usr/lib/arm-linux-gnueabihf/GL/host/libexec/droid-hybris/system/lib"
                   << ldlibs;
  fo << "--env=HYBRIS_LD_LIBRARY_PATH=" + ldlibs_processed.join(':');

  // set LD_LIBRARY_PATH to the used extension
  if (qset.value("main/set_ld_path", 1).toInt() > 0)
    fo << "--env=LD_LIBRARY_PATH=" + qset.value("main/ld_library_path",
                                                "/usr/lib/arm-linux-gnueabihf/GL/host/lib").toString();

  // add supplied flatpak options in the end
  fo << flatpak_options;

  // add program and its options
  fo << program << program_options;

  // debug
#if 1
  std::cout << "WAYLAND_DISPLAY=\"" << wayland_socket.toStdString() << "\" "
            << "FLATPAK_MALIIT_CONTAINER_DBUS=\"" << dbusaddress.toStdString() << "\" "
            << fc.toStdString() << " " << fo.join(' ').toStdString() << "\n";
#endif

  // start application
  m_process = new QProcess(this);
  m_process->setProcessEnvironment(env);

  connect(m_process, &QProcess::errorOccurred, this, &Runner::onError);
  connect(m_process, &QProcess::readyReadStandardError, this, &Runner::onStdErr);
  connect(m_process, &QProcess::readyReadStandardOutput, this, &Runner::onStdOut);
  connect(m_process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
          this, &Runner::onFinished);

  m_command = fc;
  m_options = fo;
  m_flatpak_program = program;
  emit programChanged(m_flatpak_program);
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

void Runner::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  onStdErr();
  onStdOut();

  bool c = (exitStatus == QProcess::CrashExit);
  if (m_crashed != c)
    {
      m_crashed = c;
      emit crashedChanged(m_crashed);
    }

  if (m_exitCode != exitCode)
    {
      m_exitCode = exitCode;
      emit exitCodeChanged(m_exitCode);
    }

  emit exit();
}

void Runner::onStdErr()
{
  std::cerr << m_process->readAllStandardError().constData() << std::flush;
}

void Runner::onStdOut()
{
  std::cout << m_process->readAllStandardOutput().constData() << std::flush;
}
