CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS = \
        ame_designer_plugin.h

SOURCES = \
        ame_designer_plugin.cpp

INCLUDEPATH += ../amelib /usr/include/ame/
LIBS += -L/usr/lib -L../amelib -lame
OBJECTS_DIR += ../build
MOC_DIR += ../build

TARGET = amewidgets
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

