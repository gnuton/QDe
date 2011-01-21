#ifndef __SYSMENU_H
#define __SYSMENU_H

#include "button.h"

class Panel;
class Adx;

class SystemMenu : public GenericButton
{
	Q_OBJECT
public:
	explicit SystemMenu(Adx *a, Panel *p, QWidget *parent = 0);
	virtual ~SystemMenu();

	void rebuildMenu(bool dockHiding = false);

public slots:
	void onAbout();
	void onPreferences();
	void onShowHideDock();
	void onDockPref();
	void onLogout();
	void onSleep();
	void onRestart();
	void onShutdown();

protected:
	Adx *app;

	QAction *about;
	QAction *logout;
	QAction *sleep;
	QAction *restart;
	QAction *shutdown;
	QAction *adock_on;
	QAction *adock_pref;

};

#endif
