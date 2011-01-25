#ifndef __SOUNDVOLUME_H
#define __SOUNDVOLUME_H

#include "button.h"
#include <Sound>

class Panel;
	
class SliderWidget : public QDialog
{
	Q_OBJECT
public:
	SliderWidget(QWidget *parent = 0);
	~SliderWidget();

	void setAutoRefresh(bool);

public slots:
	void onSliderRelease();
	void onSliderMove(int);
	void onSystemVolume(int);
	void updateVolume();

signals:
	void volumeChanged(int);

public:
	QSlider *slider;
	AmeSystemSound *snd;
	bool playFeedback;

private:
	bool changing;
	QTimer *timer;
};

class VolumeCtrl : public GenericButton
{
	Q_OBJECT
public:
	VolumeCtrl(Panel *p, QWidget *parent = 0);
	~VolumeCtrl();

	virtual void activate();
	virtual void deactivate();

	//virtual void mousePressEvent(QMouseEvent *e);

	SliderWidget *volumeSlider;

public slots:
	void updateIcon(int);
	void enableFeedback(bool);
};

#endif

