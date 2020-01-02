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

CONFIG += sailfishapp

DEFINES += QT_COMPOSITOR_QUICK

QT += quick qml
QT += quick-private

QT += compositor

SOURCES += \
    src/main.cpp \
    src/qmlcompositor.cpp

OTHER_FILES += \
    qml/main.qml \
    qml/MainPage.qml \
    qml/WindowContainer.qml \

DISTFILES += \
    rpm/flatpak-runner.spec

HEADERS += \
    src/qmlcompositor.h

