TEMPLATE = lib
PLUGIN = true
VERSION = 0.1

CONFIG += qt plugin x11 release
QT += dbus

LIBS += -lXrender

RESOURCES = \
        bobcat.qrc

HEADERS = \
        macros.h \
        bobcat.h

SOURCES = \
        bobcat.cpp \
        menus.cpp \
        buttons.cpp \
        toolbars.cpp

target.path += $$[QT_INSTALL_PLUGINS]/styles
INSTALLS += target
