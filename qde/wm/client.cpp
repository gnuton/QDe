//////////////////////////////////////////////////
//  File       	: client.cpp			//
//  Written by	: ludmiloff@gmail.com		//
//  Some copy/paste code from Antico project	//
//  Copyright  	: GPL2				//
//////////////////////////////////////////////////

#include "client.h"
#include "decor.h"
#include "atoms.h"
#include "dockbar.h"
#include "desktop.h"

Client::Client(Window w, Dockbar *dck, Desktop *d, int topbarHeight, bool dblClick, QWidget *parent)
	: QWidget(parent)
{
	clientId = w;
	appName = "";
	clientName = "";
	titlebar = NULL;
	
	dock = dck;
	desktop = d;

	keyboardGrabbed = false;
	dblClickMinimize = dblClick;
	tbHeight = topbarHeight;
	resizeState = false;
	
	//setAutoFillBackground(false);
	//setAttribute(Qt::WA_NoSystemBackground);
	//setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_DeleteOnClose);
	init();
}

Client::~Client()
{
}

void Client::init()
{
	XGrabServer(display());
	
	// reparent client with parent (qt)
	XSetWindowBorderWidth(display(), clientId, 0);  //client
	XSetWindowBorderWidth(display(), winId(), 0);  //parent
	
	XReparentWindow(display(), clientId, winId(), BORDER_WIDTH, Titlebar::getPreferedHeight(0));
	qDebug() << "Reparent window:" << clientId << "with Qt parent:" << winId();
	
	XSetWindowAttributes at;
	at.event_mask = DECOR_EVENT_MASK;
	XChangeWindowAttributes(display(), winId(), CWEventMask, &at);
	at.event_mask = CLIENT_EVENT_MASK;
	XChangeWindowAttributes(display(), clientId, CWEventMask, &at);
	
	XAddToSaveSet(display(), clientId);
	
	XGrabButton(display(), Button1, AnyModifier, clientId, FALSE,
			ButtonPressMask|ButtonReleaseMask, 
			GrabModeSync, GrabModeAsync, None, None);
	
	// get initial client geometry
	XWindowAttributes attr;
	XGetWindowAttributes(display(), clientId, &attr);
	n_px = attr.x;
	n_py = attr.y;
	base_w = attr.width;
	base_h = attr.height;
	fixSizes();
	
	n_pw = base_w;// + DBL_BORDER_WIDTH;
	n_ph = base_h + TITLEBAR_HEIGHT + 8;
	
	getWMHints();
	getAppName();
	getWMName();
	getWMProtocols();
	
	windowRole = Atoms::windowType(clientId);
	
	int bx, by;
	if (windowRole == Atoms::Splash) {
		bx = by = 0;
		n_ph = n_ph - TITLEBAR_HEIGHT - 8;
	} else {
		bx = BORDER_WIDTH;
		by = TITLEBAR_HEIGHT;
	}

	XMoveResizeWindow(display(), clientId, bx, by, base_w, base_h);

	// move (in desktop center) and resize parent (qt)
	n_px = (desktop->width()-base_w)/2;
	n_py = ((desktop->height()-dock->height() - n_ph)*2)/3 + tbHeight;

	qDebug() << "desktop h = " << desktop->height() << "dock h = " << dock->height();
	qDebug() << "n_px = " << n_px << " n_py = " << n_py << " base_w = " << base_w << " base_h = " << base_h;

	move(n_px, n_py);
	resize(n_pw, n_ph);
	
	createDecor();
	
	// map client
	show();
	XMapWindow(display(), clientId);
	XSync(display(), false);
	XUngrabServer(display());
	
	maximized = false;
	// TODO: get and restore initial client state
	clientState = NormalState;
}

void Client::createDecor()
{
	qDebug() << "CREATE DECORATIONS FOR ROLE " << windowRole;
	if (windowRole == Atoms::Splash) {
		titlebar = NULL;
		return;
	}
	
	layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	// center frame where client apps is shown
	Border *center = new Border(this);
	layout->addWidget(center, 1, 1);

	titlebar = new Titlebar(appIcon, clientName, windowRole);
	layout->addWidget(titlebar, 0, 0, 1, -1);
	// Window buttons actions
	if (windowRole != Atoms::Dialog) {
    	connect(qobject_cast <WinButton *>(titlebar->minBtn), SIGNAL(wMouseLeftPress()), SLOT(iconify()));
    	connect(qobject_cast <WinButton *>(titlebar->maxBtn), SIGNAL(wMouseLeftPress()), SLOT(maximize()));
    	connect(qobject_cast <WinButton *>(titlebar->closeBtn), SIGNAL(wMouseLeftPress()), SLOT(destroyClient()));
	}
	
	bottombar = new GenericBar();
	layout->addWidget(bottombar, 2, 0, 1, -1);

	// Titlebar actions
	connect(titlebar, SIGNAL(tMouseLeftPress(QMouseEvent *)), SLOT(titlebarMousePress(QMouseEvent *)));
	connect(titlebar, SIGNAL(tMouseMove(QMouseEvent *)), SLOT(titlebarMove(QMouseEvent *)));
	if (dblClickMinimize) {
		connect(titlebar, SIGNAL(tMouseDblClick()), SLOT(iconify()));
	}
	
	// Bottombar actions
	connect(bottombar, SIGNAL(tMouseLeftPress(QMouseEvent *)), SLOT(bottombarMousePress(QMouseEvent *)));
	connect(bottombar, SIGNAL(tMouseMove(QMouseEvent *)), SLOT(bottombarMove(QMouseEvent *)));
	connect(bottombar, SIGNAL(tMouseRelease()), SLOT(bottombarMouseRelease()));
	
	setLayout(layout);
}

void Client::setDblMinimizeClick(bool active)
{
	dblClickMinimize = active;
	disconnect(titlebar, SIGNAL(tMouseDblClick()), 0, 0);
	if (dblClickMinimize) {
		connect(titlebar, SIGNAL(tMouseDblClick()), SLOT(iconify()));
	}
}


void Client::fixSizes()
{	
	if (base_w >= desktop->width())
		base_w = desktop->width();

	if (desktop->width() - base_w < 10)
		base_w = desktop->width();
	
	if (base_h >= desktop->height()-dock->height() - tbHeight - 8)
        base_h = desktop->height()-dock->height() - tbHeight - 8;
	
	if (n_px <= desktop->x()) 
		n_px = desktop->x();
	if (n_py <= desktop->y() + tbHeight) 
		n_py = desktop->y() + tbHeight;
}

void Client::setDecorState(bool s)
{
	if (!titlebar) return;
	if (s) {
		titlebar->changeState(Titlebar::borderActive);
		bottombar->changeState(Titlebar::borderActive);
	} else {
		titlebar->changeState(Titlebar::borderInactive);
		bottombar->changeState(Titlebar::borderInactive);		
	}
}

void Client::iconify()
{
	unmap();
	setClientState(IconicState);
	dock->addClient(this);  // add to dockbar
}

void Client::iconifyFast()
{
	unmap();
	clientState = IconicState;
	dock->addClient(this);  // add to dockbar
}

void Client::maximize()
{
	if (! maximized) {
                // save parent dimension
		n_px = x();
		n_py = y();
		n_pw = width();
		n_ph = height();
		// maximize parent with (0,0) vertex and screen dimension-dockbar height
		m_pw = desktop->width();
		m_ph = desktop->height()-dock->height();
		move(desktop->x(), desktop->y());
		resize(m_pw, m_ph);
		// set maximized state
		maximized = true;
	} else {
		// set last parent dimension
		move(n_px, n_py);
		resize(n_pw, n_ph);
		// set not maximized state
		maximized = false;
	}
	
	raise();
	XResizeWindow(display(), clientId, width()-DBL_BORDER_WIDTH, height()-TITLEBAR_HEIGHT-8);
}

void Client::removeFromDock()
{
	qDebug() << "REMOVE CLIENT FROM DOCK";
	dock->removeClient(this);
	map();
}

void Client::map()
{
	XUngrabKeyboard(display(), CurrentTime);
	show();
	XMapWindow(display(), clientId);
	setClientState(NormalState);
	XGrabKeyboard(display(), clientId, TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
}
	
void Client::mapFast()
{
	XUngrabKeyboard(display(), CurrentTime);
	show();
	XMapWindow(display(), clientId);
	clientState = NormalState;
	XGrabKeyboard(display(), clientId, TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
}

void Client::unmap()
{
	hide();
}

void Client::destroyClient()
{
	if (protDelete) {
		qDebug() << "ON DESTROY WINDOW (PROT DELETE)";
        Atoms::sendWMProtocols(clientId, Atoms::atom(Atoms::WM_DELETE_WINDOW), CurrentTime);
	} else {
		qDebug() << "ON DESTROY WINDOW";
		XDestroyWindow(display(), clientId);
		XSync(display(), FALSE);
    }
}

//
// Window movement & resize routines
//

void Client::titlebarMousePress(QMouseEvent *event)
{
	mousepos = event->pos()+titlebar->pos();  // offset
}

void Client::titlebarMove(QMouseEvent *event)
{
	QPoint p(event->globalPos()-mousepos);
	if (p.x() <= desktop->x())
		p.setX(desktop->x());
    if (p.y() <= desktop->y())
		p.setY(desktop->y());
	move(p.x(), p.y());
}

void Client::bottombarMousePress(QMouseEvent *event)
{
	if (bottombar->cursorPos == GenericBar::RightPos)
		mousepos = event->globalPos();
	else if (bottombar->cursorPos == GenericBar::NonePos)
		mousepos = event->pos()+bottombar->pos();  // offset
	resizeState = true;
}

void Client::bottombarMouseRelease()
{
	resizeState = false;
}

void Client::bottombarMove(QMouseEvent *event)
{
	QPoint p(event->globalPos()-mousepos);
	//qDebug() << p.x() << p.y();

	if (bottombar->cursorPos == GenericBar::NonePos) {
		if (p.x() <= desktop->x())
			p.setX(desktop->x());
		if (p.y() <= desktop->y())
			p.setY(desktop->y());
		move(p.x(), p.y());
		//qDebug() << "MOVE: " << p.x() << p.y();
	} else if (bottombar->cursorPos == GenericBar::RightPos) {
		int resh = height()+p.y();
		int resw = width()+p.x();
		// FIXME: use minimal sizes supplied
		if (resw < 30) return;
		if (resh < 30) return;
		//move and resize parent
		//qDebug() << "RESIZE: " << p.x() << p.y() << resh << resw;
		XResizeWindow(display(), clientId, resw, resh-TITLEBAR_HEIGHT - 8);
		resize(resw, resh);
		mousepos = event->globalPos();
	}
}
