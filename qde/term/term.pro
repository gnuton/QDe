TEMPLATE = app
QT += xml dbus
CONFIG += qt release


HEADERS +=

SOURCES += termmain.cpp

INCLUDEPATH += ../amelib /usr/include/ame/
LIBS += -L/usr/lib -L../amelib -lame
OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += /usr/include

QMAKE_CLEAN += runner.app
TARGET = term.app
target.path = /usr/bin
INSTALLS += target
