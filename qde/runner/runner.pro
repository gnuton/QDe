TEMPLATE = app
QT += xml dbus
CONFIG += qt release

RESOURCES = runner.qrc

UI_HEADERS_DIR = forms
FORMS += 

HEADERS += singleapp.h \
	runner.h \
	runnerwindow.h \
	sidebar.h \
	preview.h \
	listview.h \
	colview.h \
	folderview.h

SOURCES += singleapp.cpp \
	runner.cpp \
	runnerwindow.cpp \
	sidebar.cpp \
	preview.cpp \
	listview.cpp \
	colview.cpp \
	folderview.cpp \
	runnermain.cpp

INCLUDEPATH += ../amelib /usr/include/ame/
LIBS += -L/usr/lib -L../amelib -lame
OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += /usr/include forms/

QMAKE_CLEAN += runner.app
TARGET = runner.app
target.path = /usr/bin
INSTALLS += target
