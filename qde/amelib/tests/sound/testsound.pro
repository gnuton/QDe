TEMPLATE = app
CONFIG += qt debug
QT += phonon
RESOURCES = test.qrc

INCLUDEPATH += /usr/include/ame/
LIBS += -L/usr/lib -lame

HEADERS += testsound.h	

SOURCES += 	testsound.cpp 

TARGET = testsound.app
