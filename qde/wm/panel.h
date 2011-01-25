//////////////////////////////////////////
//  File       : panel.h		//
//  Written by : ludmiloff@gmail.com	//
//  Copyright  : GPL2			//
//////////////////////////////////////////
#ifndef __PANEL_H
#define __PANEL_H

#include "defs.h"
#include <PixmapWidget>

#define TOP_PANEL_HEIGHT 23

class Adx;
class Client;
class GenericButton;
class CurrentApp;
class ShowDesktop;
class VolumeCtrl;
class SystemMenu;
class KbSwitch;
class Systray;
class PowerIndicator;

class Panel : public AmePixmapWidget
{

public:
	explicit Panel(Adx *a, QWidget *parent=0);
	virtual ~Panel(void);

	// various helper routines for Antico WM integration
	bool isFocused();
	void grabFocus();
	bool unfocusChilds();

	void deactivateLast(GenericButton *c);
	void setCurrentWidget(GenericButton *c, WId id);
	void clearCurrentWidget();
	WId getActiveWidgetId();

	void iconIsRemoved(Client *client);
	void windowIsIconified(Client *client);

	// Panel widgets - public for speedup access
	SystemMenu *sysBtn;
	ShowDesktop *desktopBtn;
	VolumeCtrl *volume;
	CurrentApp *currentApp;
        KbSwitch *kbswitch;
        Systray *tray;
	PowerIndicator *pwrIndicator;

	// DBus callable function
	void enableSoundVolumeFeedback(bool);
	void showSoundVolumeCtrl(bool);
	void showKbdCtrl(bool);
	void changeSoundDevices(const QString &card, const QString &mixer);
private:

	QRect screen(int indx=-1);
	int calcWidth(void);
	void setupGui(void);

protected:
	// The design goals is to provide only 
	// top horizontal panel
	QHBoxLayout *layout;

	bool m_Focus;
	GenericButton *currentWidget;
	WId activeWidgetId;

	// General update/refresh timer
	QTimer *m_Timer;

	virtual void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *event);

	Adx *app;
};


#endif

