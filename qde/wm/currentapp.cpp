#include "currentapp.h"
#include "panel.h"
#include "adx.h"

CurrentApp::CurrentApp(Adx *a, Panel *p, QWidget *parent)
	 : GenericButton(p, parent)
{
	app = a;
	client = NULL;
	iconified = false;
	allIconified = false;
        setMenu(new QMenu());
}

CurrentApp::~CurrentApp()
{
}

void CurrentApp::rebuildMenu()
{
        menu()->clear();
	if (!client) {
		hideAll = new QAction("Hide All", this);
		hideAll->setShortcut(Qt::CTRL+Qt::ALT+Qt::Key_F9);
		menu()->addAction(hideAll);
		connect(hideAll, SIGNAL(triggered()), this, SLOT(onHideAll()));
	} else {
		QAction *hideThis;
		if (client->clientState == NormalState) {
			hideThis = new QAction("Hide " + text(), this);
			iconified = false;
		} else {
			hideThis = new QAction("Show " + text(), this);
			iconified = true;
		}
		hideThis->setShortcut(Qt::ALT+Qt::Key_F9);
		menu()->addAction(hideThis);
		connect(hideThis, SIGNAL(triggered()), this, SLOT(onHideThis()));
		hideOthers = new QAction("Hide Others", this);
		connect(hideOthers, SIGNAL(triggered()), this, SLOT(onHideOthers()));
		menu()->addAction(hideOthers);
	}

	showAll = new QAction("Show All", this);
	connect(showAll, SIGNAL(triggered()), this, SLOT(onShowAll()));
	menu()->addAction(showAll);

}

Client *CurrentApp::getCurrent()
{
	return client;
}

Window CurrentApp::getCurrentWindow()
{
	if (client != NULL)
		return client->clientId;
	else
		return 0;
}

void CurrentApp::onHideThis()
{
	if (client) {
		QAction *action = qobject_cast<QAction *>(sender());
		//qDebug() << "CURRENT APP - HIDE THIS";
		if (!iconified) {
			//qDebug() << "ICONIFY";
			iconified = true;
			client->iconify();
			action->setText("Show " + text());
		} else {
			//qDebug() << "SHOW";
			iconified = false;
			client->removeFromDock();
			action->setText("Hide " + text());
		}
	}
}

void CurrentApp::onHideOthers()
{
	app->minimizeOthers(client);
}

void CurrentApp::onHideAll()
{
	app->minimizeAll();	
}

void CurrentApp::onShowAll()
{
	app->showAll();
}

void CurrentApp::setCurrent(const QString &name, Client *c)
{
	if (client && client != c) {
		client->setDecorState(false);
		if (c)
			c->setDecorState(true);
	} else if (!client && c) {
		c->setDecorState(true);
	}
	client = c;
	setText(name);
	iconified = false;
	rebuildMenu();
}

void CurrentApp::setIconified(const QString &name, Client *c)
{
	if (client && client != c) {
		client->setDecorState(false);
		if (c)
			c->setDecorState(true);
	}
	client = c;
	setText(name);
	iconified = true;
	rebuildMenu();
}

