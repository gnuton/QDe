TEMPLATE = app
VERSION = 0.1.98

QT += dbus xml
CONFIG += qt release

RESOURCES = theme/std.qrc

UI_HEADERS_DIR = forms
FORMS += \
        forms/frm_quit.ui \
        forms/frm_about.ui

OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += forms/
QMAKE_CLEAN += antico-deluxe

INCLUDEPATH += ../amelib /usr/include/ame/
LIBS += -L../amelib -lame -lX11

HEADERS += defs.h \
        aboutdlg.h \
        quitdlg.h \
        alttab.h \
        atoms.h \
        adx.h \
        dbusadaptor.h \
        decor.h \
        client.h \
        desktop.h \
        dockicon.h \
        dockbar.h \
        button.h \
        menu.h \
        currentapp.h \
        sysmenu.h \
        showdesktop.h \
        clock.h \
        volumectrl.h \
        kbswitch.h \
        systray.h \
        panel.h \
    powerindicator.h

SOURCES += main.cpp \
        aboutdlg.cpp \
        quitdlg.cpp \
        alttab.cpp \
        atoms.cpp \
        adx.cpp \
        adxsettings.cpp \
        keybindings.cpp \
        dbusadaptor.cpp \
        events.cpp \
        decor.cpp \
        client.cpp \
        x11management.cpp \
        panel.cpp \
        desktop.cpp \
        dockicon.cpp \
        dockbar.cpp \
        button.cpp \
        menu.cpp \
        currentapp.cpp \
        sysmenu.cpp \
        showdesktop.cpp \
        clock.cpp \
        volumectrl.cpp \
        kbswitch.cpp \
        systray.cpp \
    powerindicator.cpp

TARGET = antico-deluxe
target.path=/usr/bin

theme_files.files += theme/default/ame-logo.png \
	theme/default/ame-logo-active.png \
	theme/default/ame-logo1.png \
	theme/default/dockbar.png \
	theme/default/dockicon.png \
	theme/default/application.png \
	theme/default/header.png \
	theme/default/show-desk.png \
	theme/default/show-desk-active.png

theme_files.path = /usr/share/themes/antico/default

session_gdm_files.files += anticodeluxe-gdm.desktop
session_gdm_files.path=/usr/share/xsessions

session_kdm_files.files += anticodeluxe-kdm.desktop
session_kdm_files.path=/usr/share/apps/kdm/sessions/

exec_files.files += scripts/antico-deluxe-session
exec_files.path=/usr/bin

INSTALLS += target exec_files theme_files session_gdm_files session_kdm_files
