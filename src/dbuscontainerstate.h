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

  Q_PROPERTY(int activeState READ activeState NOTIFY activeStateChanged)
  Q_PROPERTY(int orientation READ orientation NOTIFY orientationChanged)

public:
  DBusContainerState(const QWindow *window);

  QString address() const { return m_server->address(); }

  bool activeState() const;
  int orientation() const { return m_orientation; }

signals:
  void activeStateChanged(bool state);
  void orientationChanged(int orientation);

protected:
  void onActiveState(Qt::ApplicationState state);
  void onNewConnection(const QDBusConnection &c);
  void onWindowOrientationChanged(Qt::ScreenOrientation orientation);

protected:
  QDBusServer *m_server;

  int m_orientation;
  const QWindow *m_window;
};

#endif // DBUSCONTAINERSTATE_H
