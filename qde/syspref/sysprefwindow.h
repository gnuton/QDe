#ifndef __SYSPREF_H
#define __SYSPREF_H

#include <QtGui>
#include <StaticIconList>
#include <ToolBar>
#include "browser.h"

class PrefWidget;
class DBusAdaptor;

class SysPref : public QMainWindow
{
	Q_OBJECT
public:
	SysPref(int module);
	~SysPref();

	void createToolbar();
	void setupLaunchers();
	void addModule();
	void discardPrevious();
	void discardNext();

	void remoteFunction(int fEnumerator, int param);

public slots:
	void onShowMain();
	void onShowPrev();
	void onShowNext();
	void onWallpaper();
	void onAppearance();
	void onDock();
	void onDateTime();
	void onSound();
        void onIntnl();

protected:
	void closeEvent(QCloseEvent *event);

private:
	AmeStaticIconList *personal, *hardware, *network, *system;
	Browser *browser;
	AmeToolBar *toolbar;
        AmeRectButton *prev, *next, *showAll;
	QStackedWidget *stack;
	PrefWidget *prevWidget, *nextWidget;
	// DBus control
	QObject obj;
	DBusAdaptor *WMCtrl;
};

#endif
