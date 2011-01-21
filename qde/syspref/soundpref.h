//////////////////////////////////////////
//  File       	: soundpref.h		//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL2			//
//////////////////////////////////////////

#ifndef __SOUNDPREF_H
#define __SOUNDPREF_H

#include <QtGui>
#include "prefwidget.h"
#include "ui_sound.h"

class hwDeviceInfo
{
public:
	int card;
	QString cardName;
	int device;
	QString deviceName;

	QString fullName() const;
};

typedef QMap<QString, hwDeviceInfo *> hwDevices;

class SoundPref : public PrefWidget
{
	Q_OBJECT
public:
	enum DeviceMode {modePlayback, modeCapture};

	SoundPref(QWidget *parent = 0);
	~SoundPref();

	virtual void refresh();
	virtual void readSettings();
	virtual bool saveSettings();

public slots:
	void onSlider(int);
	void onSliderRelease();
	void onMute();
	void onVolumeFeedback();
	void onShowHideVolumeCtrl();
	void checkVolume();
	void onSoundItemChange(QTreeWidgetItem *, int);
	void onSoundItem(QTreeWidgetItem *, int);
	void onMixerDevice(const QString &);
	void onChangeDevices();

private:
	Ui::soundFrm ui;
	AmeSettings *stg1;

	QString mixerCard, mixerDevice;
	QTimer *timer;

	hwDevices *hwList;
	void getHwList(int);
	void getMixerDevices(int);
};

#endif
