#include "dbuscontainerstate.h"

#include <QDBusConnection>
#include <QGuiApplication>
#include <QScreen>

#include <QDebug>


DBusContainerState::DBusContainerState(const QWindow *window):
  QObject(), m_window(window)
{
  // follow application state
  connect(qGuiApp, &QGuiApplication::applicationStateChanged, this, &DBusContainerState::onActiveState);
  onActiveState(qGuiApp->applicationState());

  // follow orientation
  connect(m_window, &QWindow::contentOrientationChanged, this, &DBusContainerState::onWindowOrientationChanged);
  onWindowOrientationChanged(m_window->contentOrientation());

  // create dbus server
  m_server = new QDBusServer(this);
  connect(m_server, &QDBusServer::newConnection, this, &DBusContainerState::onNewConnection);
}


static int orientationAngle(Qt::ScreenOrientation orientation)
{
  // From input-context/minputcontext.cpp of Maliit Framework

  // Maliit uses orientations relative to screen, Qt relative to world
  // Note: doesn't work with inverted portrait or landscape as native screen orientation.
  static bool portraitRotated = qGuiApp->primaryScreen()->primaryOrientation() == Qt::PortraitOrientation;

  switch (orientation) {
  case Qt::PrimaryOrientation: // Urgh.
  case Qt::PortraitOrientation:
      return portraitRotated ? 0 : 270;
  case Qt::LandscapeOrientation:
      return portraitRotated ? 90 : 0;
  case Qt::InvertedPortraitOrientation:
      return portraitRotated ? 180 : 90;
  case Qt::InvertedLandscapeOrientation:
      return portraitRotated ? 270 : 180;
  }
  return 0;
}


bool DBusContainerState::activeState() const
{
  return (qGuiApp->applicationState() == Qt::ApplicationActive);
}


void DBusContainerState::onActiveState(Qt::ApplicationState state)
{
  bool s = (state == Qt::ApplicationActive);
  emit activeStateChanged(s);
  // update window orientation if the application became active
  if (s)
    onWindowOrientationChanged(m_window->contentOrientation());
}

void DBusContainerState::onWindowOrientationChanged(Qt::ScreenOrientation orientation)
{
  m_orientation = orientationAngle(orientation);
  emit orientationChanged(m_orientation);
}


void DBusContainerState::onNewConnection(const QDBusConnection &c)
{
  QDBusConnection connection(c);
  connection.registerObject("/", this, QDBusConnection::ExportAllProperties | QDBusConnection::ExportAllSignals);
  connection.registerService(FLATPAK_RUNNER_DBUS_CONT_SERVICE);
}
