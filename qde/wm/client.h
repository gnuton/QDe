//////////////////////////////////////////
//  File       	: client.h		//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL			//
//////////////////////////////////////////

#ifndef __CLIENT_H
#define __CLIENT_H

#include "defs.h"

#define CLIENT_EVENT_MASK \
        ColormapChangeMask|\
        PropertyChangeMask|\
        StructureNotifyMask|\
        FocusChangeMask

#define DECOR_EVENT_MASK \
        ButtonPressMask|ButtonReleaseMask|\
        KeymapStateMask|\
        ButtonMotionMask|PointerMotionMask|\
        EnterWindowMask|LeaveWindowMask|FocusChangeMask|\
        ExposureMask|StructureNotifyMask|SubstructureRedirectMask|SubstructureNotifyMask

class Client;
class Titlebar;
class GenericBar;
class Dockbar;
class Desktop;

typedef QHash<int, Client *> ClientList;

class Client : public QWidget
{
        Q_OBJECT
public:
        Client(Window w, Dockbar *dck, Desktop *d, int topbarHeight=23, bool dblClick=true, QWidget *parent = 0);
        ~Client();

        void init();
        void createDecor();
        void setDecorState(bool);

        // X11 management routines
        void getWMNormalHints();
        void getWMHints();
        void getWMName();
        void getAppName();
        void getWMProtocols();
        void getServerIcon(Pixmap icon, Pixmap mask);
        void getColormaps(void);
        void setColormaps(Colormap cmap);
        void setClientState(int state);
        void fixSizes();
        void resizeRequest(int cx, int cy, int cw, int ch);
        void setChildFocus(Window subwindow, long timestamp);
        void delayedGrab(int);

        // User/system management routines
public slots:
        void iconify();
        void iconifyFast();
        void maximize();
        void map();
        void mapFast();
        void unmap();
        void removeFromDock();
        void destroyClient();
        // Window movement & resize
        void titlebarMousePress(QMouseEvent *);
        void titlebarMove(QMouseEvent *);
        void bottombarMousePress(QMouseEvent *);
        void bottombarMove(QMouseEvent *);
        void bottombarMouseRelease();

        void doGrab();
        void setDblMinimizeClick(bool);

public:
        QString clientName;			// client title/iconic name
        Window clientId;			// client x11 window id
        QPixmap appIcon;			// client window icon
        int clientState;

        bool protDelete;			// client has WM_DELETE protocol
        bool protTakeFocus;			// client has WM_TAKE_FOCUS protocol
        bool wantFocus;				// does this application rely on the window manager to get keyboard input?
        bool Urgency;

        int windowRole;				// window role (Normal, Dialog, Splash, ...)

        Colormap colorMap;			// client colormap
        long wmnflags;				// XWMNormalHints
        int wingrav;				// XWMNormalHints

        int base_w, base_h;			// XWMNormalHints
        int inc_w, inc_h;			// XWMNormalHints
        int max_w, max_h;			// XWMNormalHints
        int min_w, min_h;			// XWMNormalHints

        int client_w, client_h;                 // client geometry

        QString	appName;			// Application class name
        QString icnName;			// WM_ICON_NAME

        // Decorations widgets
        QGridLayout *layout;
        Titlebar *titlebar;
        GenericBar *bottombar;

        int m_px, m_py, m_pw, m_ph;             // maximize state parent (qt) geometry
        int n_px, n_py, n_pw, n_ph;             // normal state parent (qt) geometry
        bool maximized;

        bool keyboardGrabbed;
        bool dblClickMinimize;

        // DE widgets
        Desktop *desktop;
        Dockbar *dock;

        // internals
        int tbHeight;				// Topbar height
        QPoint mousepos;
        bool resizeState;
};

#endif
