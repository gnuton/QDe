#ifndef __CURRENTAPP_H
#define __CURRENTAPP_H

#include "defs.h"
#include "button.h"

class Adx;
class Client;
class Panel;

class CurrentApp : public GenericButton
{
	Q_OBJECT
public:
	CurrentApp(Adx *a, Panel *p, QWidget *parent = 0);
	~CurrentApp();

	void rebuildMenu();

	void setCurrent(const QString &name, Client *c);
	void setIconified(const QString &name, Client *c);
	Client *getCurrent();
	Window getCurrentWindow();

public slots:
	void onHideThis();
	void onHideAll();
	void onShowAll();
	void onHideOthers();

protected:
	
	Adx *app;
	Client *client;

	bool iconified, allIconified;
	
	QAction *hideThis;
	QAction *hideOthers; 
	QAction *hideAll;
	QAction *showAll;	
};

#endif

