/////////////////////////////////////////
//  File       : dockbar.cpp           //
//  Written by : g_cigala@virgilio.it  //
//  Modified by: ludmiloff@gmail.com   //
//  Copyright  : GPL                   //
/////////////////////////////////////////

// create the dockbar on bottom of the screen

#ifndef DOCKBAR_H
#define DOCKBAR_H

#include "defs.h"

#define MAX_DOCK_HEIGHT 64

class Client;
class DockIcon;
class Adx;

typedef QList<DockIcon *> DockIconsList;

class Dockbar : public QLabel
{
    Q_OBJECT

public:

	enum DockState {
		Normal,
		Hidden,
		Hiding,
		Showing,
		AboutToShow
	};

	Dockbar(Adx *a, QWidget *parent=0);
	~Dockbar();

	// Show/ hide speed
	void setAnimSpeed(int);

	// Show / Hide flags
	void setAutoHide(bool active=true);
	int dockState;
	bool autoHide;
	
	// Client management functions
	void addClient(Client *);
	bool removeClient(Client *);
	bool removeAll();

	// Launchers management functions
	void addLauncher(const QString &file);
	
	void setSizeFactor(int factor, bool doSave=true);
	void updateSize();
	void readSettings();
	void saveSettings();
	
	int actualHeight();

public slots:
	void removeIcon(DockIcon *);

	// Show / Hide functions
	void showFast();
	void hideShowDock();
	void animateHide();
	void animateShow();
	void hide1Step();
	void show1Step();

signals:
	void clientRemoved(Client *);

protected:
        virtual void dragEnterEvent(QDragEnterEvent *event);
        virtual void dropEvent(QDropEvent *event);

private:
	QHBoxLayout *dockLayout;
	QHBoxLayout *iconLayout;
	QFrame *dockFrame;
	Adx *app;

	DockIconsList *iconsList;
	QString dockPix;	//
	int dockSizeFactor; 	// Dockbar scale factor (0,100) percents
	int dockAnimFactor;	//Speed of hiding/showing.
	int delay, speed;
    
	QTimer *timer, *hideTimer;
	bool checkCursor();
};
#endif
 
