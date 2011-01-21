CONFIG += qt warn_on release

INCLUDEPATH += /usr/local/include/pineos/
LIBS += -L/usr/local/lib -lpineos
HEADERS += 	

SOURCES += 	testresources.cpp 

TARGET = testresources

