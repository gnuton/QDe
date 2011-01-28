//////////////////////////////////////////
//  File       	: adx.cpp		//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL			//
//////////////////////////////////////////

#include <QtDBus>
#include "adx.h"
#include "atoms.h"
#include "aboutdlg.h"
#include "quitdlg.h"
#include "dbusadaptor.h"
#include "sysmenu.h"

#define WM_DBUS_SERVICE "org.freedesktop.AnticoDeluxe"

#define HAL_DOMAIN "org.freedesktop.Hal"
#define HAL_PATH "/org/freedesktop/Hal/devices/computer"
#define HAL_SERVICE "org.freedesktop.Hal.Device.SystemPowerManagement"

Adx::Adx(int &argc, char **argv) : QApplication(argc, argv),
    WMCtrl(0),
    hal(0),
    stg(0),
    alttab(0),
    client(0)
{	
	m_Process = process_Initializing;
	Atoms::createNetWMAtoms(display());

	setOrganizationName("AnticoDeluxe");
	setApplicationName("AnticoDeluxe");
	readAdxSettings();

	XGrabKeyboard(display(), rootWindow(), TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
	
	desktop = new Desktop(this);
	desktop->show();

	QTimer::singleShot(0, this, SLOT(init()));
}	

Adx::~Adx()
{
}

void Adx::init()
{
    dock = new Dockbar(this);
    dock->show();

    //FIXME Crash if toppanel is created before dock
    toppanel = new Panel(this);
    //desktop->init(TOP_PANEL_HEIGHT);
    toppanel->show();

    toppanel->currentApp->setCurrent("Desktop", 0);

    // Initialize HAL DBus interfaces
    WMCtrl = new DBusAdaptor(this, desktop, dock, &obj);
    QDBusConnection::sessionBus().registerObject("/", &obj);
    if (QDBusConnection::sessionBus().registerService(WM_DBUS_SERVICE)) {
	QDBusConnection bus = QDBusConnection::systemBus();
	hal = new QDBusInterface(HAL_DOMAIN, HAL_PATH, HAL_SERVICE, bus);
    } else {
	qWarning() << "UNABLE TO REGISTER DBUS SERVICE";
    }

    toppanel->sysBtn->rebuildMenu(dock->autoHide);
    manageRunningClients();
    m_Process = process_Normal;
}

// Show Antico Deluxe "About this..." dialog
void Adx::onAbout()
{
	QColor col = QApplication::palette().color(QPalette::Highlight);
	AboutDlg *dlg = new AboutDlg(MAJOR_VERSION, MINOR_VERSION, QString(MODIFICATION));
	sysId = dlg->winId();	
	dlg->exec();
	delete dlg;
	sysId = 0;
	// quick workaround "Highlight color disappear" bug
	setHighlightColor(col);
}


// Window management routines
void Adx::minimizeOthers(Client *thisClient)
{
	m_Process = process_MinimizingWindows;
	foreach (client, clients)
	{
		// TODO: check frame/window type. Do not iconify 
		// some types like splash screen etc.
		if (client->clientState == NormalState && client->winId() != thisClient->winId())
			client->iconifyFast();
	}
	thisClient->setChildFocus(0, CurrentTime);
	m_Process = process_Normal;
}

void Adx::minimizeAll()
{
	m_Process = process_MinimizingWindows;
	foreach(client, clients) {
		// TODO: check frame/window type. Do not iconify
		// some types like splash screen etc.
		if (client->clientState == NormalState)
			client->iconifyFast();
	}
	toppanel->currentApp->setCurrent("Desktop", NULL);
	XUngrabKeyboard(display(), CurrentTime);
	XGrabKeyboard(display(), rootWindow(), TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
	
	m_Process = process_Normal;
}

void Adx::showAll()
{
	dock->removeAll();
}

void Adx::onDockIconRemoved(Client *client)
{
	toppanel->iconIsRemoved(client);
	XRaiseWindow(display(), client->winId());
	restack(client);
	toppanel->currentApp->setCurrent(client->appName, client);
	client->setChildFocus(0, CurrentTime);
}

void Adx::manageRunningClients()
{
	qDebug() << "Get already running clients ...";
	Window c_win, w1, w2, *wins;
	uint nwins, cwin;
	XWindowAttributes attr;
	if (XQueryTree(display(), rootWindow(), &w1, &w2, &wins, &nwins) == 0 || ! nwins) {
		qDebug() << "No already running clients ...";
		return;
	}
	
	for (cwin=0; cwin < nwins; cwin++) {
		c_win = wins[cwin];
		//qDebug() << "run process:" << cwin << "name:" << c_win;
		if (isSystemWidget(c_win))
			continue;
		XGetWindowAttributes(display(), c_win, &attr);
		
		if ((client = clients.value(c_win)) != NULL) {
			//qDebug() << "map process:" << cwin << "name:" << c_win;
			client->map();
		}
		
		else if (attr.map_state == IsViewable && ! attr.override_redirect) {
			//qDebug() << "create process:" << cwin << "name:" << c_win;
			createMyWindow(c_win);
		}
	}
}

void Adx::unmanageRunningClients()
{
}

void Adx::focusTopmostClient()
{
	Window c_win, w1, w2, *wins;
	uint nwins;
	if (XQueryTree(display(), rootWindow(), &w1, &w2, &wins, &nwins) == 0 || ! nwins) {
		//qDebug() << "No running clients ...";
		return;
	}
	
	for (int cwin=nwins-1; cwin >= 0; cwin--) {
		c_win = wins[cwin];
		//qDebug() << "run process:" << cwin << "name:" << c_win;
		
		if ((client = decors.value(c_win)) != NULL ||
			(client = clients.value(c_win)) != NULL) {
			if (client->clientState == NormalState) {
				toppanel->currentApp->setCurrent(client->appName, client);
				client->setDecorState(true);
				XRaiseWindow(display(), client->clientId);
				restack(client);				
				client->setChildFocus(0, CurrentTime);
				break;
			}
		}
	}
}

// Computer management routines
void Adx::onLogout()
{
	QColor col = QApplication::palette().color(QPalette::Highlight);
	QuitDlg *dlg = new QuitDlg(QuitDlg::fLogout);
	sysId = dlg->winId();
	int f = dlg->exec();
	delete dlg;
	sysId = 0;
	if (f == QDialog::Accepted) {
		wmCleanup();
		quit();
	}
	// quick workaround "Highlight color disappear" bug
	setHighlightColor(col);
}

void Adx::onPCSleep()
{
    if (!hal) return;
    hal->call("Suspend", 0);
}

void Adx::onPCRestart()
{
    if (!hal) return;
	QColor col = QApplication::palette().color(QPalette::Highlight);
	QuitDlg *dlg = new QuitDlg(QuitDlg::fReboot);
	sysId = dlg->winId();
	int f = dlg->exec();
	delete dlg;
	sysId = 0;
	if (f == QDialog::Accepted) {
		wmCleanup();
		hal->call("Reboot");
		quit();
	}
	// quick workaround "Highlight color disappear" bug
	setHighlightColor(col);
}

void Adx::onPCShutdown()
{
    if (!hal) return;
	QColor col = QApplication::palette().color(QPalette::Highlight);
	QuitDlg *dlg = new QuitDlg(QuitDlg::fHalt);
	sysId = dlg->winId();
	int f = dlg->exec();
	delete dlg;
	sysId = 0;
	if (f == QDialog::Accepted) {
		wmCleanup();
		hal->call("Shutdown");
		quit();
	}
	// quick workaround "Highlight color disappear" bug
	setHighlightColor(col);
}

void Adx::setHighlightColor(const QColor &col)
{
	QPalette pal = QApplication::palette();
	pal.setColor(QPalette::Highlight, col);
	QApplication::setPalette(pal);
}

void Adx::setMinimizeOnDblClick(bool active)
{
	minimizeDblClick = active;
	foreach(client, clients) {
		client->setDblMinimizeClick(active);
	}
}

void Adx::wmCleanup()
{
	foreach(client, clients) {
		qDebug() << "Quit process:" << client->clientId;
		XRemoveFromSaveSet(display(), client->clientId);
		XReparentWindow(display(), client->clientId, rootWindow(), client->n_px, client->n_py);
		client->destroyClient();
	}

	clients.clear();
	decors.clear();

	dock->close();
	toppanel->close();
	XSync(display(), FALSE);
	flush();
}


