//////////////////////////////////////////////////////////
//  File       	: events.cpp				//
//  Written by	: ludmiloff@gmail.com			//
//  Some copy/paste code from original Antico project	//
//  Copyright  	: GPL					//
//////////////////////////////////////////////////////////

#include "adx.h"
#include "atoms.h"
#include "alttab.h"
#include "kbswitch.h"
#include "systray.h"
#include "compmgr.h"

/* defined in the systray spec */
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2

Adx *deluxe;
bool replay;
bool resend = false;

bool Adx::x11EventFilter(XEvent *event)
{	

	if (compositorMgr)
	    compositorMgr->eventFilter(event);

	//qDebug() << "X11 EVENT FILTER " << event->type;
	switch (event->type) {
		
		case KeyPress:
			qDebug() << "*** KEY PRESS *** window: " << event->xkey.window;
			if (!resend) deluxe->onKeyPress(event);
			resend = false;
			return false;
			break;
		
		case KeyRelease:
			qDebug() << "*** KEY RELEASE *** window: " << event->xkey.window;
			if (!resend) deluxe->onKeyRelease(event);
			resend = false;
			return false;
			break;
		
		case ButtonPress:
			qDebug() << "*** BUTTON PRESS *** window: " << event->xbutton.window << "subwindow: " << event->xbutton.window;
			return deluxe->onButtonPress(event);
			break;
		
		case ConfigureNotify:
			//qDebug() << "*** CONFIGURE NOTIFY *** window: " << event->xbutton.window;
			if (event->xconfigure.event != event->xconfigure.window)
				return true;
			return false;
			break;
		
		case ConfigureRequest:
			qDebug() << "*** CONFIGURE REQUEST *** window: " << event->xbutton.window;
			return deluxe->configureRequest(event);
			break;
		
		case MapRequest:
			//qDebug() << "*** MAP REQUEST *** window: " << event->xmaprequest.window;
			deluxe->createMyWindow(event->xmaprequest.window);
			return false;
			break;
		
		case MapNotify:
			if (event->xmap.window == deluxe->sysId) {
				XSetInputFocus(display(), event->xmap.window, None, CurrentTime);
				XRaiseWindow(display(), event->xmap.window);
			}
			if (event->xmap.event != event->xmap.window)
				return true;
			else
				return false;
			break;
		
		case UnmapNotify:
                        //qDebug() << "*** UNMAP NOTIFY *** window: " << event->xunmap.window;
			return deluxe->unmapWindow(event);
			break;
		
		case DestroyNotify:
                        //qDebug() << """ DESTROY NOTIFY *** window: " << event->xdestroywindow.window;
			return deluxe->destroyWindow(event);
			break;
		
		case PropertyNotify:
			return deluxe->onX11Property(event);
			break;
		
		case ColormapNotify:
			return deluxe->colormapNotify(event);
			break;
		
		case FocusIn:
			//qDebug() << "FOCUS IN EVENT" << event->xfocus.mode << event->xfocus.detail << event->xany.window;
			deluxe->onFocusIn(event);
			return false;
			break;

                case ClientMessage:
                        qDebug() << "X11 Client message" << event->xclient.message_type;
                        if (event->xclient.message_type == Atoms::atom(Atoms:: NET_SYSTEM_TRAY_OPCODE)) {

                                if (event->xclient.data.l[1] == SYSTEM_TRAY_REQUEST_DOCK) {
                                        qDebug() << "SYSTEM_TRAY_REQUEST_DOCK";
                                        deluxe->onAddTrayIcon(event->xclient.data.l[2]);
                                } else
                                if (event->xclient.data.l[1] == SYSTEM_TRAY_BEGIN_MESSAGE) {
                                        qDebug() << "SYSTEM_TRAY_BEGIN_MESSAGE";
                                } else
                                if (event->xclient.data.l[1] == SYSTEM_TRAY_CANCEL_MESSAGE) {
                                        qDebug() << "SYSTEM_TRAY_CANCEL_MESSAGE";
                                }
                                return true;
                                break;
                        }
                        else if (event->xclient.data.l[1] == (signed)Atoms::atom(Atoms::NET_SYSTEM_TRAY_MESSAGE_DATA)) {
                                qDebug() << "Text message from Dockapp:" << event->xclient.data.b;
                        }
                        return false;
                        break;
			
	}

	return false;
}

void Adx::onAddTrayIcon(Window w)
{
        toppanel->tray->addEmbed(w);
}

void Adx::onRemoveTrayIcon(Window w)
{
        toppanel->tray->removeEmbed(w);
}

void Adx::sendKeyEvent(XEvent *event, long mask)
{
	Window w;
	// redirect Key Press/Release event
	w = toppanel->currentApp->getCurrentWindow();
	if ( w != 0) {
		event->xkey.window = w;
		resend = true;
		XSendEvent(display(), w, TRUE, mask, event);
	}
	XAllowEvents(display(), SyncBoth, CurrentTime);
}

bool Adx::onKeyPress(XEvent *event)
{
	KeySym sym;
	uint mod;
	uint keymask1 = Mod1Mask & 0x0F;
	bool ishotkey = false;
	sym = (int)XLookupKeysym(&event->xkey, 0);
	mod = event->xkey.state & keymask1 ;

	qDebug() << sym << mod;
	if (keygrab) { // a hot key activation
		ishotkey = onHotKey(sym, mod);
		if (ishotkey) return true;
	}
	if (ctrlgrab && sym == XK_Shift_L) {
		qDebug() << "CTRL+SHIFT";
		toppanel->kbswitch->nextLayout();
	}

	if (sym == XK_Control_L && !ctrlgrab) {
		ctrlgrab = true;
		return true;
	}
	if (sym == XK_Alt_L && !keygrab) {
		keygrab = true;
		qDebug() << "ALT KEY ACTIVATION";
		return true;
	} else { 
		// redirect Key Press event
		sendKeyEvent(event, KeyPressMask);
		return false;
	}
}

bool Adx::onKeyRelease(XEvent *event)
{
	KeySym sym;
	uint mod;
	uint keymask1 = Mod1Mask & 0x0F;
	
	sym = (int)XLookupKeysym(&event->xkey, 0);
	mod = event->xkey.state & keymask1 ;
	if (sym == XK_Alt_L && keygrab) {
		keygrab = false;

		if (alttab != NULL && m_Process == process_SwitchingWindows) {
			alttab->hide();
			client = alttab->selectedIcon();
	
			if (client) {
				if (client->clientState == IconicState) {
					client->map();
					dock->removeClient(client);
					toppanel->iconIsRemoved(client);
				}
				toppanel->currentApp->setCurrent(client->appName, client);
				client->setDecorState(true);
				XRaiseWindow(display(), client->clientId);
				restack(client);				
				client->setChildFocus(0, CurrentTime);
			}

			m_Process = process_Normal;
		}

		return true;
	} else {
		// redirect Key Release event
		sendKeyEvent(event, KeyReleaseMask);
		return false;
	}
}

bool Adx::onButtonPress(XEvent *event)
{
	replay = false;
	//qDebug() << "is system widget=" << isSystemWidget(event->xbutton.window);
	if (event->xbutton.window == desktop->winId()) { // (isSystemWidget(event->xbutton.window)) {  // event on desk
		XUngrabKeyboard(display(), CurrentTime);
		XSetInputFocus(display(), event->xbutton.window, None, CurrentTime);
		toppanel->unfocusChilds();
		toppanel->currentApp->setCurrent("Desktop", NULL);
		XGrabKeyboard(display(), rootWindow(), TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
	} else {
		if ((client = clients.value(event->xbutton.window)) != NULL ||
			(client = decors.value(event->xbutton.window)) != NULL)
		{
			XRaiseWindow(display(), client->winId());
			restack(client);
			client->setChildFocus(0, event->xbutton.time);
			toppanel->currentApp->setCurrent(client->appName, client);
			replay = true;
		} else {
			XRaiseWindow(display(), event->xbutton.window);
			if ((client = toppanel->currentApp->getCurrent()) != NULL) {
				XSetInputFocus(display(), client->clientId, None, CurrentTime);
				restack(client);
				client->setChildFocus(0, event->xbutton.time);
				toppanel->currentApp->setCurrent(client->appName, client);
			} else {
				XSetInputFocus(display(), event->xbutton.window, None, CurrentTime);
				XUngrabKeyboard(display(), CurrentTime);
				XGrabKeyboard(display(), rootWindow(), TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
			}
		}
	}
	XAllowEvents (display(), replay ? ReplayPointer : SyncPointer, CurrentTime);
	return false;
}

bool Adx::onFocusIn(XEvent *event)
{
	if ((client = clients.value(event->xany.window)) != NULL) {
		client->doGrab();
	}
	return false;
}

bool Adx::configureRequest(XEvent *event)
{
	XWindowChanges wc;
	if ((client = clients.value(event->xconfigurerequest.window)) != NULL) {
		qDebug() << "--> configure (map) request p:" << client->winId() << "c:" << event->xconfigurerequest.window;
			
		if (event->xconfigurerequest.value_mask & (CWWidth|CWHeight|CWX|CWY)) {
			int cx,cy,cw,ch;
			if (event->xconfigurerequest.value_mask & CWWidth)
				cw = event->xconfigurerequest.width;
			else
				cw = client->base_w;

			if (event->xconfigurerequest.value_mask & CWHeight)
				ch = event->xconfigurerequest.height;
			else
				ch = client->base_h;

			if ((event->xconfigurerequest.value_mask & CWX))
				cx = event->xconfigurerequest.x;
			else
				cx = client->n_px;

			if ((event->xconfigurerequest.value_mask & CWY))
				cy = event->xconfigurerequest.y;
			else
				cy = client->n_py;
			
			client->resizeRequest(cx, cy, cw, ch);
			event->xconfigurerequest.value_mask &= ~(CWWidth|CWHeight|CWX|CWY);
		}

		if (! event->xconfigurerequest.value_mask)
			return true;

		/*
        wc.width = frm->width();
        wc.height = frm->height();
        wc.x = frm->x();
        wc.y = frm->y();
        wc.border_width = 0;
        wc.sibling = event->xconfigurerequest.above;
        wc.stack_mode = event->xconfigurerequest.detail;
        XConfigureWindow(display(), frm->winId(), event->xconfigurerequest.value_mask, &wc);
        send_configurenotify(frm);
		*/
	} else { // never mapped window
		qDebug() << "--> configure (never map) request c:" << event->xconfigurerequest.window;
		wc.x = event->xconfigurerequest.x;
		wc.y = event->xconfigurerequest.y;
		wc.width = event->xconfigurerequest.width;
		wc.height = event->xconfigurerequest.height;
		event->xconfigurerequest.value_mask &= (CWX|CWY|CWWidth|CWHeight);
		XConfigureWindow(display(), event->xconfigurerequest.window, event->xconfigurerequest.value_mask, &wc);
	}
	return true;
}

bool Adx::unmapWindow(XEvent *event)
{
	if ((client = clients.value(event->xunmap.window)) != NULL) {
		client->unmap();
		return true;
	}
	if (event->xunmap.window == toppanel->getActiveWidgetId()) {
		toppanel->unfocusChilds();
	}
	if (event->xunmap.event != event->xunmap.window)
		return true;

	return false;
}

bool Adx::destroyWindow(XEvent *event)
{
	if ((client = clients.value(event->xdestroywindow.window)) != NULL) {
		clients.remove(event->xdestroywindow.window);
		decors.remove(client->winId());
		toppanel->currentApp->setCurrent("Desktop", NULL);
		client->close();
		QTimer::singleShot(100, this, SLOT(focusTopmostClient()));
		return true;
	}

        // try to remove try icon, if exists
        toppanel->tray->removeEmbed(event->xdestroywindow.window);

	if (event->xdestroywindow.event != event->xdestroywindow.window)
		return true;
	
	return false;
}

bool Adx::onX11Property(XEvent *event)
{
	//qDebug() << "PROPERTY";
	//qDebug() << "Atom: " << XGetAtomName(QX11Info::display(), event->xproperty.atom) << '\n';
	//qDebug() << event->xproperty.window;
	
	if ((client = clients.value(event->xproperty.window)) != NULL ||
		(client = decors.value(event->xproperty.window)) != NULL) {
		if (event->xproperty.atom == Atoms::atom(Atoms::WM_NORMAL_HINTS)) {
			qDebug() << """ PROPERTY NOTIFY *** wm_normal_hints";
			client->getWMNormalHints();
		} 
		else if (event->xproperty.atom == Atoms::atom(Atoms::WM_HINTS)) {
			qDebug() << """ PROPERTY NOTIFY *** wm_hints";
			client->getWMHints();
		}
		else if (event->xproperty.atom == Atoms::atom(Atoms::NET_WM_NAME) || 
			     event->xproperty.atom == Atoms::atom(Atoms::NET_WM_ICON_NAME)) {
			qDebug() << """ PROPERTY NOTIFY *** wm_name";
			client->getWMName();	
		}
		else if (event->xproperty.atom == Atoms::atom(Atoms::NET_WM_STATE) ||
				 event->xproperty.atom == Atoms::atom(Atoms::WM_STATE)) {
			qDebug() << """ PROPERTY NOTIFY *** wm_state";
			int state = client->clientState;
			if (state == NormalState) {
				XSetInputFocus(display(), client->clientId, None, CurrentTime);
			} else if (state == IconicState) {
				toppanel->windowIsIconified(client);
			}
		}
		else if (event->xproperty.atom == Atoms::atom(Atoms::WM_COLORMAP_WINDOWS)) {
			//qDebug() << """ PROPERTY NOTIFY *** colormaps";
			client->getColormaps();
		}
		else if (event->xproperty.atom == Atoms::atom(Atoms::WM_TRANSIENT_FOR)) {
			qDebug() << """ PROPERTY NOTIFY *** transients";
			client->map();
			client->raise();
			//XGrabServer(display());
		}
		else if (event->xproperty.atom == Atoms::atom(Atoms::NET_WM_USER_TIME)) {
			//qDebug() << """ PROPERTY NOTIFY *** wm_user_time";
			return false;
		}
		
		return true;
	}
	return false;
}

void Adx::createMyWindow(Window w)
{
	if ((client = clients.value(w)) != NULL) {
		client->map();
	} else {
		client = new Client(w, dock, desktop, toppanel->height(), minimizeDblClick);
		clients.insert(w, client);
		decors.insert(client->winId(), client);
		toppanel->currentApp->setCurrent(client->appName, client);
		client->setChildFocus(0, CurrentTime);
		restack(client);
	}
}

bool Adx::colormapNotify(XEvent *event)
{
	if ((client = clients.value(event->xcolormap.window)) != NULL) {
		client->setColormaps(event->xcolormap.colormap);
		return true;
	}
	return false;
}

void Adx::onAltTab()
{
	if (decors.isEmpty())
		return;
	if (alttab != NULL) {
		delete alttab;
		alttab = NULL;
	}
	
	alttab = new AltTabDlg(this, toppanel->currentApp->getCurrent(), &decors, desktop);
	//qDebug() << "ALT-TAB ID=" << alttab->winId();
	m_Process = process_SwitchingWindows;
	keygrab = true;
	alttab->show();
	XSetInputFocus(display(), alttab->winId(), None, CurrentTime);
	XRaiseWindow(display(), alttab->winId());
}

bool Adx::isSystemWidget(Window w)
{
	if (w == desktop->winId()) return true;
	if (w == toppanel->winId()) return true;
	if (w == dock->winId()) return true;
	if (w == sysId) return true;
	return false;
}

void Adx::restack(Client *client)
{
	Window wins[2];
	wins[0] = dock->winId();
	wins[1] = client->winId();
	XRestackWindows(display(), wins, 2);
}
