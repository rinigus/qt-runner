# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = flatpak-runner

# PREFIX
isEmpty(PREFIX) {
    PREFIX = /usr
}

target.path = $$PREFIX/bin

CONFIG += sailfishapp

DEFINES += QT_COMPOSITOR_QUICK
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT += quick qml svg
QT += quick-private

QT += dbus

QT += compositor

SOURCES += \
    src/appsettings.cpp \
    src/dbuscontainerstate.cpp \
    src/imageconverter.cpp \
    src/main.cpp \
    src/qmlcompositor.cpp \
    src/runner.cpp

OTHER_FILES += \
    qml/main.qml \
    qml/AboutPage.qml \
    qml/MainPage.qml \
    qml/LabelC.qml \
    qml/Python.qml \
    qml/Space.qml \
    qml/WindowContainer.qml \
    scripts/flatpak-extension-hybris \
    fpk/*.py

DISTFILES += \
    rpm/flatpak-runner.spec

HEADERS += \
    src/appsettings.h \
    src/dbuscontainerstate.h \
    src/imageconverter.h \
    src/qmlcompositor.h \
    src/runner.h

scripts.files = scripts
scripts.path = $$PREFIX/share/$${TARGET}
INSTALLS += scripts

fpk.files = fpk
fpk.path = $$PREFIX/share/$${TARGET}
INSTALLS += fpk

icons.files = icons
icons.path = $$PREFIX/share/$${TARGET}
INSTALLS += icons

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256
DISTFILES += $${TARGET}.desktop
