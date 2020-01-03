# Flatpak Runner

This is a helper program for running Flatpak applications
on Sailfish.


## Usage

To run Flatpak application, start

```
flatpak-runner [flatpak_run_opt1 flatpak_run_opt2 ...] application [app_opt_1 app_opt_2 ...]
```

where `application` is a full Flatpak application ID. You can specify
Flatpak run and application command line options as optional arguments
to `flatpak-runner`. Options specified before application are
considered as Flatpak run options.

For libhybris support, you have to generate Flatpak extension. For that,
run `flatpak-extension-hybris` as a local user (nemo). This has to be run
every time you update libhybris on your device (usually after each Sailfish
update).

When installing Flatpak applications, use `--user` options to install it in
your home folder.


## Mode of operation

`flatpak-runner` starts a separate Wayland compositor with the first
available Wayland socket. It sets `WAYLAND_DISPLAY` varaible in the
environment for the launched `flatpak run` command. In addition, it
sets multiple options

* setting DPI to the physical DPI for Qt applications (can be
  overwritten in settings file)
* mounting `/system` and `/vendor` in read-only mode for libhybris
* allowing access to all devices
* setting hybris environment variables

If `flatpak-runner` is called without any arguments, it will start
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


## Acknowledgments

The code is based on QtWayland [example
compositor](https://github.com/qt/qtwayland/tree/5.4/examples/wayland/qml-compositor)
and adaptation of it for Sailfish:
[qxcompositor](https://github.com/elros34/qxcompositor)

