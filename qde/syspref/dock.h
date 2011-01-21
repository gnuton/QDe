#ifndef __DOCK_H
#define __DOCK_H

#include <QtGui>
#include "prefwidget.h"
#include "ui_dock.h"

class DockPref : public PrefWidget
{
	Q_OBJECT
public:
	DockPref(QWidget *parent = 0);
	~DockPref();

	virtual void readSettings();
	virtual bool saveSettings();

	virtual void execFunction(int, int);

public slots:
	void onChangeDockSize(int value);
	void onShowHideDock(bool active);
	void onDockAnimation(int);

protected:

private:
	Ui::dockFrm ui;
};

#endif
