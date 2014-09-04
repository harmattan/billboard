
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

DEPENDPATH += ../billboardd/
INCLUDEPATH += ../billboardd/

SOURCES += main.cc

SOURCES += glue.cc
HEADERS += glue.h

HEADERS += ../billboardd/renderer.h
HEADERS += ../billboardd/formatter.h

SOURCES += ../billboardd/dithering.cc
HEADERS += ../billboardd/dithering.h

