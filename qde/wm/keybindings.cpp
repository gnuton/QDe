#include "adx.h"
#include "kbswitch.h"

#define ALT_MOD 8
#define CTRL_MOD 4
#define SHIFT_MOD 1
#define MAC_MOD 64

bool Adx::onHotKey(KeySym sym, uint mod)
{
	bool hot = false;
	
	qDebug() << "HOTKEY ACTIVATED";
	switch (sym) {
	    case XK_F4:{
		qDebug() << "F4";
		if (mod != ALT_MOD) break;
		hot = true;
		if ((client = toppanel->currentApp->getCurrent()) != NULL) {
		    client->destroyClient();
		}
	    }break;
		
	    case XK_F9:{
		qDebug() << "F9";
		if (mod != ALT_MOD) break;
		hot = true;
		if ((client = toppanel->currentApp->getCurrent()) != NULL) {
		    if (client->clientState == NormalState) {
			qDebug() << "HIDING WINDOW";
			client->iconify();
			toppanel->windowIsIconified(client);
			XSetInputFocus(display(), rootWindow(), None, CurrentTime);
		    } else if (client->clientState == IconicState) {
			qDebug() << "SHOW WINDOW";
			client->removeFromDock();
			toppanel->iconIsRemoved(client);
			client->setChildFocus(0, CurrentTime);
			restack(client);
		    }
		}
	    }break;

	    case XK_Tab:{
		qDebug() << "TAB";
		if (mod != ALT_MOD) break;
		if (m_Process != process_SwitchingWindows) {
		    QTimer::singleShot(220, this, SLOT(onAltTab()));
		}
		hot = true;
	    }break;

	    case XK_Shift_L:{
		qDebug() << "ALT+SHIFT";
		if (mod != ALT_MOD) break;
		toppanel->kbswitch->nextLayout();
	    }break;

	}

	XAllowEvents(display(), SyncBoth, CurrentTime);
	return hot;
}
