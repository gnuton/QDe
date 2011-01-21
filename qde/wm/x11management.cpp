#include "client.h"
#include "dockbar.h"
#include "atoms.h"
#include "decor.h"

void Client::getWMNormalHints()
{

    XSizeHints *xsizehints;
    if ((xsizehints = XAllocSizeHints()) == NULL)
        sleep(1);

    long hints;
    if (XGetWMNormalHints(display(), clientId, xsizehints, &hints) == 0)
        xsizehints->flags = 0;

    wmnflags = xsizehints->flags;
	qDebug() << "WINDOW FLAGS = " << wmnflags;
    wingrav = xsizehints->win_gravity;

    bool pminsize = false;

    // set max, min and base size (results include window borders)

/*
    if (wmnflags & PMinSize)
    {
		qDebug() << "PMINSIZE";
        min_w = xsizehints->min_width;
        min_h = xsizehints->min_height+diff_border_h;
        pminsize = true;   // to get base size if unspecified
    }
    else
    {
        min_w = 4;
        min_h = diff_border_h;
    }
    if (wmnflags & PMaxSize)
    {
		qDebug() << "PMAXSIZE";
        max_w = xsizehints->max_width;
        max_h = xsizehints->max_height+diff_border_h;

        if (max_w > desk->width()-10)
            max_w = desk->width()-10;

        if (max_h > desk->height()-dock_height)
            max_h = desk->height()-dock_height;
    }
    else
    {
        max_w = desk->width()-10;
        max_h = desk->height()-dock_height;
    }
*/

/*
    if (wmnflags & PSize || wmnflags || USSize)
    {
		qDebug() << "PSIZE";
        base_w = xsizehints->base_width;
        base_h = xsizehints->base_height + diff_border_h;

        if (! pminsize) // get from base if unspecified
        {
            min_w = base_w;
            min_h = base_h;
        }
    }
    else if (pminsize)
    {
        base_w = xsizehints->min_width;
        base_h = xsizehints->min_height;
    }

    if (wmnflags & PResizeInc)
    {
        inc_w = xsizehints->width_inc;
        inc_h = xsizehints->height_inc;
    }
    else
        inc_w = inc_h = 1;

	// Some window generate bad values !!!
	if (base_w > 0 && base_h > 0)
    	resize_request(n_px, n_py, base_w, base_h);

    qDebug() << "min_w:" << min_w << "min_h:" << min_h << "max_w:" << max_w << "max_h:" << max_h << "base_w:" << base_w << "base_h:" << base_h;
    XFree(xsizehints);
*/
}

void Client::getWMHints()
{
	XWMHints *xwmhints;
	if ((xwmhints = XGetWMHints(display(), clientId)) != NULL) {
		if (xwmhints->flags & StateHint && xwmhints->initial_state == IconicState) {
			setClientState(IconicState);
		}
		if (xwmhints->flags & IconPixmapHint) {
			getServerIcon(xwmhints->icon_pixmap, (xwmhints->flags & IconMaskHint)?(xwmhints->icon_mask):(None));
		}
		if (appIcon.isNull())
			appIcon = QPixmap(":/default/appicon.png");
		// FIXME: UrgencyHint not defined
		//Urgency = xwmhints->flags & UrgencyHint;
		wantFocus = xwmhints->input;
		XFree(xwmhints);
    }
}

void Client::getWMName()
{
	char *name;
	Atom type;
	int format;
	unsigned long nitems=0;
	unsigned long extra=0;
	unsigned char *data=NULL;

	if (XFetchName(display(), clientId, &name) && name != NULL) {
        clientName = QString::fromUtf8((char *)name);
        XFree(name);
    } 	else {
		XGetWindowProperty(display(), clientId, Atoms::atom(Atoms::NET_WM_NAME),
			0, 500, FALSE, AnyPropertyType, &type, &format, &nitems, &extra, &data);

		if (data != NULL) {
			clientName = QString::fromUtf8((char *)data);
			XFree(data);
    	} else  // use class hints
			clientName = appName;
	}
	data = NULL;
	XGetWindowProperty(display(), clientId, Atoms::atom(Atoms::NET_WM_ICON_NAME),
		0, 500, FALSE, AnyPropertyType, &type, &format, &nitems, &extra, &data);
	
	if (data != NULL) {
		icnName = QString::fromUtf8((char *)data);
		XFree(data);
	} else if (XGetIconName(display(), clientId, &name) && name != NULL) {
		icnName = name;
		XFree(name);
	} else
		icnName = clientName;
	
	if (titlebar != NULL) titlebar->setTitle(clientName);
	qDebug() << "WINDOW NAME: " << clientName;
}

void Client::getAppName(void)
{
	XClassHint c;
	c.res_name = NULL;
	c.res_class = NULL;
	XGetClassHint(display(), clientId, &c);
	if (c.res_class != NULL)
		appName = QString::fromUtf8((char *)c.res_class);
	else if (c.res_name != NULL)
		appName = QString::fromUtf8((char *)c.res_class);
	else
		appName = clientName;
	XFree(c.res_name);
	XFree(c.res_class);
}

void Client::getWMProtocols(void)
{
	Atom *protocols;
	int nprot,i;

	protDelete = false;
	protTakeFocus = false;

	if (XGetWMProtocols(display(), clientId, &protocols, &nprot)) {
		for (i=0; i < nprot; i++) {
            if (protocols[i] == Atoms::atom(Atoms::WM_DELETE_WINDOW)) { 
				protDelete = true;
			} else if (protocols[i] == Atoms::atom(Atoms::WM_TAKE_FOCUS)) {
				protTakeFocus = true;
			}
		}
		XFree(protocols);
	}
}

void Client::setClientState(int state)
{
    ulong data[2];
    data[0] = (ulong)state;
    data[1] = (ulong)None;
	clientState = state;
    XChangeProperty(display(), clientId, Atoms::atom(Atoms::WM_STATE),
                    Atoms::atom(Atoms::WM_STATE), 32, PropModeReplace, (uchar *)data, 2);
	//qDebug() << "CLIENT STATE =" << clientState;
}

void Client::getServerIcon(Pixmap icon, Pixmap mask)  // get pixmap from server and scale it
{
	int ix, iy;
	uint iw, ih, bwidth, depth;
	Window w;
	GC gc;
	
	if (!XGetGeometry(display(), icon, &w, &ix, &iy, &iw, &ih, &bwidth, &depth)) {
		// no icon defined - set default icon
		appIcon = QPixmap(":/default/appicon.png");
		return;
	}
	QPixmap pix(iw, ih);
	pix.detach();
	gc = XCreateGC(display(), icon, 0, 0);
	XCopyArea(display(), icon, pix.handle(), gc, 0, 0, iw, ih, 0, 0);
	XFreeGC(display(), gc);
	
	if (mask != None) {
		gc = XCreateGC(display(), mask, 0, 0);
		QBitmap bmap(iw, ih);
		bmap.detach();
		XCopyArea(display(), mask, bmap.handle(), gc, 0, 0, iw, ih, 0, 0);
		pix.setMask(bmap);
		XFreeGC(display(), gc);
	}
	
	appIcon = pix.scaledToHeight(dock->height(), Qt::SmoothTransformation);
}

void Client::getColormaps(void)
{
	XWindowAttributes attr;
	XGetWindowAttributes(display(), clientId, &attr);
	colorMap = attr.colormap;
}

void Client::setColormaps(Colormap cmap)
{
	if (cmap== None)
		colorMap = DefaultColormap(display(), DefaultScreen(display()));
	XInstallColormap(display(), colorMap);
}

void Client::resizeRequest(int cx, int cy, int cw, int ch)
{
	if (resizeState)
		return;
	base_w = cw;
	base_h = ch;
	
	n_px = cx;
	n_py = cy+TITLEBAR_HEIGHT+8;

	fixSizes();

	n_pw = base_w;
	n_ph = base_h + TITLEBAR_HEIGHT + 8;

    //move(n_px, n_py);
    resize(n_pw, n_ph);
    XResizeWindow(display(), clientId, base_w, base_h);
	qDebug() << "DONE RESIZE (IN REQUEST) WINDOW";
}

void Client::delayedGrab(int t)
{
	XUngrabKeyboard(display(), CurrentTime);
	keyboardGrabbed = false;
	QTimer::singleShot(t, this, SLOT(doGrab()));
}

void Client::doGrab()
{
	int ret = 0;
	if (!keyboardGrabbed)
		ret = XGrabKeyboard(display(), clientId, TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
	if (ret !=0)
		keyboardGrabbed = false;
	else
		keyboardGrabbed = true;
}

// set to active and focus to child
void Client::setChildFocus(WId subwindow, long timestamp)
{
	if (subwindow == 0) XSetInputFocus(display(), clientId, RevertToPointerRoot, CurrentTime);

    if (protTakeFocus) { // WM_TAKE_FOCUS protocol 
        Atoms::sendWMProtocols(clientId, Atoms::atom(Atoms::WM_TAKE_FOCUS), timestamp);
		qDebug() << "PROT. TAKE FOCUS";
	}
	XUngrabKeyboard(display(), CurrentTime);
	keyboardGrabbed = false;
	QTimer::singleShot(220, this, SLOT(doGrab()));
}
