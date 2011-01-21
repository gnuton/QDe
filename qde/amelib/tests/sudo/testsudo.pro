TEMPLATE = app
CONFIG += qt release

INCLUDEPATH += /usr/include/ame/
LIBS += -L/usr/lib -lame
HEADERS += testsudo.h	

SOURCES += 	testsudo.cpp 

TARGET = testsudo
