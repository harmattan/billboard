
CONFIG += mobility link_pkgconfig
PKGCONFIG += dbus-1 dbus-glib-1

MOBILITY += systeminfo organizer

QT += dbus

TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += link_pkgconfig
PKGCONFIG += gq-gconf contextsubscriber-1.0

SOURCES += billboardd.cc
SOURCES += organizer.cc
HEADERS += daemon.h
HEADERS += context.h
HEADERS += gconfmon.h
HEADERS += batteryicon.h

HEADERS += renderer.h
HEADERS += formatter.h

SOURCES += dithering.cc
HEADERS += dithering.h

SOURCES += network.cc
HEADERS += network.h

SOURCES += tklockinterface.cc
HEADERS += tklockinterface.h

SOURCES += volumeinterface.cc
HEADERS += volumeinterface.h

target.path = /opt/billboard/bin
INSTALLS += target

launch.path = /opt/billboard/bin
launch.files = billboardd.launch
INSTALLS += launch

upstart.path = /etc/init/apps
upstart.files = billboardd.conf
INSTALLS += upstart

