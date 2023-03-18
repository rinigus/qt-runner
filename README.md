# Qt Runner

This is a helper program for running Qt 5.15 applications
on Sailfish.


## Usage

To run Qt application, start

```
qt-runner application [app_opt_1 app_opt_2 ...]
```

where `application` is a full Qt application path. You can specify
application command line options as optional arguments
to `qt-runner`.


## Mode of operation

`qt-runner` starts a separate Wayland compositor with the first
available Wayland socket. It sets `WAYLAND_DISPLAY` varaible in the
environment for the launched application. In addition, it
sets multiple options

* setting DPI to the physical DPI for Qt applications (can be
  overwritten in settings file)

If `qt-runner` is called without any arguments, it will start
Wayland server with the first available socket and will print its
location at standard output.


## Limitations

Currently, it supports apps that work with "wl-shell" Wayland shell
extension. This is depricated extension and Gdk seems to not support
it while Qt does.

As there is no window management in the implemented Wayland
compositor, the app has to be without dialog boxes with all contained
within one window.

In part, the limitations are imposed by old QtWayland (5.4) available
on Sailfish.


## Translations

Not setup yet.

## Acknowledgments

Qt Runner is based on [Flatpak
Runner](https://github.com/sailfishos-flatpak/flatpak-runner). The
code is based on QtWayland [example
compositor](https://github.com/qt/qtwayland/tree/5.4/examples/wayland/qml-compositor)
and adaptation of it for Sailfish:
[qxcompositor](https://github.com/elros34/qxcompositor).

