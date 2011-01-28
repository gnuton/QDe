//////////////////////////////////////////
//  File       	: adx.cpp		//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL			//
//////////////////////////////////////////

#ifndef __ADX_H
#define __ADX_H

#include <Settings>
#include "defs.h"
#include "client.h"
#include "desktop.h"
#include "dockbar.h"
#include "panel.h"
#include "currentapp.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define MODIFICATION "98-1"

class DBusAdaptor;
class QDBusInterface;
class AltTabDlg;
class compmgr;

class Adx : public QApplication
{
	Q_OBJECT

public:
	enum AdxProcess {
		process_Initializing,
		process_Normal,
		process_MinimizingWindows,
		process_RestoringWindows,
		process_Wait,
		process_CreateWindow,
		process_SwitchingWindows,
	};
	
	Adx(int &argc, char **argv);
	~Adx();
	void wmCleanup();
	void readAdxSettings();

	// DE widgets
	Panel *toppanel;
	Desktop *desktop;
	Dockbar *dock;
	Window sysId;	// system widget/dialog Window id

	// X11 event filter
	bool x11EventFilter(XEvent* event);
	//bool x11_event_filter(void *message, long *result);
	
	// X11 event management helper routines
	void createMyWindow(Window w);
	bool destroyWindow(XEvent *);
	bool unmapWindow(XEvent *);
	bool configureRequest(XEvent *);
	bool onFocusIn(XEvent *);
	bool onButtonPress(XEvent *);
	bool onHotKey(KeySym sym, uint mod);
	bool onKeyPress(XEvent *);
	bool onKeyRelease(XEvent *);
	void sendKeyEvent(XEvent *, long);
	bool onX11Property(XEvent *);
	bool colormapNotify(XEvent *);
	bool isSystemWidget(Window);
	void restack(Client *);
        void onAddTrayIcon(Window);
        void onRemoveTrayIcon(Window);

	// Window management routines
	void minimizeOthers(Client *);
	void minimizeAll();
	void showAll();
	void onDockIconRemoved(Client *);

public slots:
	void init();
	void onAltTab();
	void focusTopmostClient();
	void manageRunningClients();
	void unmanageRunningClients();

public:
	// Computer management routines
	void onLogout();
	void onPCSleep();
	void onPCRestart();
	void onPCShutdown();
	
	// Show Antico Deluxe "About this..." dialog
	void onAbout();
	
	// helper routines
	void setHighlightColor(const QColor &);
	void setMinimizeOnDblClick(bool);

	// mapped clients & decorations
	ClientList clients;
	ClientList decors;

	// internals
	int m_Process;
	QObject obj;
	DBusAdaptor *WMCtrl;
	bool minimizeDblClick;
	QColor lastColor;
	QDBusInterface *hal;
	AmeSettings *stg;
	AltTabDlg *alttab;
	bool keygrab, ctrlgrab;
	Client *client;
	compmgr *compositor;
};

#endif
