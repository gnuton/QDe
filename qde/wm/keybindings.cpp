#include "adx.h"
#include "kbswitch.h"

bool Adx::onHotKey(KeySym sym, uint mod)
{
	bool hot = false;
	
	qDebug() << "HOTKEY ACTIVATED";
	switch (sym) {
		case XK_F4:
			qDebug() << "F4";
			hot = true;
			if ((client = toppanel->currentApp->getCurrent()) != NULL) {
				client->destroyClient();
			}
			break;
		
		case XK_F9:
			qDebug() << "F9";
			hot = true;
			if ((client = toppanel->currentApp->getCurrent()) != NULL) {
				if (client->clientState == NormalState) {
					qDebug() << "HIDING WINDOW";
					client->iconify();
					toppanel->windowIsIconified(client);
					XSetInputFocus(display(), rootWindow(), None, CurrentTime);
				} else
				if (client->clientState == IconicState) {
					qDebug() << "SHOW WINDOW";
					client->removeFromDock();
					toppanel->iconIsRemoved(client);
					client->setChildFocus(0, CurrentTime);
					restack(client);
				}
			}
			break;
			
		case XK_Tab:
			qDebug() << "TAB";
			if (m_Process != process_SwitchingWindows) {
				QTimer::singleShot(220, this, SLOT(onAltTab()));
			}
			hot = true;
			break;

		
	}

	if (sym == XK_Shift_L) {
		qDebug() << "ALT+SHIFT";
		toppanel->kbswitch->nextLayout();
	}
	XAllowEvents(display(), SyncBoth, CurrentTime);
	return hot;
}
