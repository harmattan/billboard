
#CONFIG += debug

QT += declarative

TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

INCLUDEPATH += ../include

CONFIG += link_pkgconfig
PKGCONFIG += gq-gconf

SOURCES += main.cc

HEADERS += controller.h

target.path = /opt/billboard/bin
INSTALLS += target

icon.path = /opt/billboard
icon.files = billboard-ui.png
INSTALLS += icon

desktop.path = /usr/share/applications
desktop.files = billboard-ui.desktop
INSTALLS += desktop

RESOURCES += billboard-ui.qrc

# Build translation files
system(make -C i18n release)
