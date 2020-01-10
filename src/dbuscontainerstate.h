#ifndef DBUSCONTAINERSTATE_H
#define DBUSCONTAINERSTATE_H

#include <QDBusServer>
#include <QWindow>
#include <QObject>

#define FLATPAK_RUNNER_DBUS_CONT_IFACE "org.container"
#define FLATPAK_RUNNER_DBUS_CONT_SERVICE "org.flatpak.sailfish.container"

class DBusContainerState: public QObject
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", FLATPAK_RUNNER_DBUS_CONT_IFACE)
  Q_PROPERTY(int orientation READ orientation NOTIFY orientationChanged)

public:
  DBusContainerState(const QWindow *window);

  QString address() const { return m_server->address(); }

  int orientation() const { return m_orientation; }

signals:
  void orientationChanged(int orientation);

protected:
  void onNewConnection(const QDBusConnection &c);
  void onWindowOrientationChanged(Qt::ScreenOrientation orientation);

protected:
  QDBusServer *m_server;

  int m_orientation;
  const QWindow *m_window;
};

#endif // DBUSCONTAINERSTATE_H
