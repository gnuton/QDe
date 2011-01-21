TEMPLATE = lib
VERSION = 0.2

QT += xml dbus phonon
CONFIG += warn_on \
    qt \
    thread \
    dll \
    x11 \
    release \
    link_pkgconfig

PKGCONFIG += alsa \


RESOURCES = amelib.qrc
UI_HEADERS_DIR = forms
FORMS += forms/password.ui
OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += \
        forms/

INCLUDEPATH += 

MAIN_HEADERS += \
        global.h \
        utils.h \
        mime.h \
        soundtheme.h \
        sound.h \
        user.h \
        simplerc.h \
        settings.h \
        desktopfile.h \
        amedirs.h \
        clickablelabel.h \
        selectablewidget.h \
        navbaritem.h \
        navbargroup.h \
        navbar.h \
        helpbutton.h \
        rectbutton.h \
        gradbutton.h \
        toolbar.h \
        pixmapwidget.h \
        systemicon.h \
        icontheme.h \
        icon.h \
        staticiconlist.h \
        passworddlg.h \
        app.h

EXTRA_HEADERS +=

HEADERS += $$MAIN_HEADERS \
    $$EXTRA_HEADERS

FLAT_HEADERS += \
        Global \
        Utils \
        Mime \
        SoundTheme \
        Sound \
        User \
        SimpleRC \
        Settings \
        DesktopFile \
        AmeDirs \
        ClickableLabel \
        SelectableWidget \
        NavBarItem \
        NavBarGroup \
        NavBar \
        HelpButton \
        RectButton \
        GradientButton \
        ToolBar \
        PixmapWidget \
        SystemIcon \
        Icon \
        StaticIconList \
        PasswordDlg \
        IconTheme \
        AmeApp

SOURCES += \
        utils.cpp \
        mime.cpp \
        soundtheme.cpp \
        sound.cpp \
        user.cpp \
        simplerc.cpp \
        settings.cpp \
        desktopfile.cpp \
        amedirs.cpp \
        clickablelabel.cpp \
        selectablewidget.cpp \
        navbaritem.cpp \
        navbargroup.cpp \
        navbar.cpp \
        helpbutton.cpp \
        rectbutton.cpp \
        gradbutton.cpp \
        toolbar.cpp \
        pixmapwidget.cpp \
        systemicon.cpp \
        icontheme.cpp \
        icon.cpp \
        staticiconlist.cpp \
        passworddlg.cpp \
        app.cpp

SHARE_FILES += \
        sounds/Click.ogg \
        sounds/Error1.ogg \
        sounds/Error2.ogg \
        sounds/Clap.ogg \
        sounds/Maximize.ogg \
        sounds/Minimize.ogg \
        sounds/Popup.ogg \
        sounds/Question.ogg \
        sounds/Information.ogg \
        sounds/Restore_up.ogg \
        sounds/Restore_down.ogg \
        sounds/LockItem.ogg \
        sounds/Shade.ogg \
        sounds/Status.ogg \
        sounds/README

TARGET = ame
target.path = /usr/lib

flat_headers.files += $$MAIN_HEADERS
flat_headers.files += $$FLAT_HEADERS
flat_headers.path = /usr/include/ame


share_files.files += $$SHARE_FILES
share_files.path = /usr/share/ame/sounds/default

INSTALLS += target
INSTALLS += flat_headers
INSTALLS += share_files
