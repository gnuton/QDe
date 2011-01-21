//////////////////////////////////////////
//  File       	: dbusadaptor.cpp	//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL2			//
//////////////////////////////////////////

#include "dbusadaptor.h"
#include "desktop.h"
#include "dockbar.h"
#include "sysmenu.h"
#include "kbswitch.h"
#include "adx.h"

DBusAdaptor::DBusAdaptor(Adx *a, Desktop *d, Dockbar *dck, QObject *obj)
	: QDBusAbstractAdaptor(obj)
{
	app = a;
	desktop = d;
	dock = dck;
}

DBusAdaptor::~DBusAdaptor()
{
}

void DBusAdaptor::changeWallpaper(const QString &path)
{
	desktop->loadWallpaper(path);
}

void DBusAdaptor::changeStyle(const QString &style)
{
	QApplication::setStyle(QStyleFactory::create(style));
}

void DBusAdaptor::changeHighlight(int r, int g, int b)
{
	app->setHighlightColor(QColor(r, g, b));
}

void DBusAdaptor::minimizeDblClick(bool active)
{
	app->setMinimizeOnDblClick(active);
}

void DBusAdaptor::changeDockSize(int factor)
{
	dock->setSizeFactor(factor);
}

void DBusAdaptor::setDockAutoHide(bool active)
{
	dock->setAutoHide(active);
	app->toppanel->sysBtn->rebuildMenu(active);
}

void DBusAdaptor::changeDockSpeed(int animFactor)
{
	dock->setAnimSpeed(animFactor);
}

void DBusAdaptor::showDockFast()
{
	dock->showFast();
}

void DBusAdaptor::soundVolumeFeedback(bool enable)
{
	app->toppanel->enableSoundVolumeFeedback(enable);
}

void DBusAdaptor::showSoundVolumeCtrl(bool enable)
{
	app->toppanel->showSoundVolumeCtrl(enable);
}

void DBusAdaptor::changeSoundDevices(const QString &card, const QString &mixer)
{
	app->toppanel->changeSoundDevices(card, mixer);
}

void DBusAdaptor::changeKbdModel(const QString &model)
{
	app->toppanel->kbswitch->onChangeKbdModel(model);
}

void DBusAdaptor::changeKbdLayouts()
{
	app->toppanel->kbswitch->readSettings();
	app->toppanel->kbswitch->displayLayout();
}

void DBusAdaptor::showInputMenu(bool enable)
{
	app->toppanel->showKbdCtrl(enable);

}
