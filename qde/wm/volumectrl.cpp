#include <SoundTheme>
#include "volumectrl.h"
#include "panel.h"

SliderWidget::SliderWidget(QWidget *parent)
	:  QDialog(parent)
{	
	QFont f = QFont(font());
	//f.setBold(true);
	f.setWeight(75);
	f.setPointSize(14);
	setFont(f);
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(5, 2, 5, 5);
	
	QLabel *plus = new QLabel("+");
	plus->setFont(f);
	plus->setAlignment(Qt::AlignHCenter);
	layout->addWidget(plus);
	
	slider = new QSlider(Qt::Vertical);
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->setSpacing(0);
	hlayout->setContentsMargins(0, 0, 0, 0);
	hlayout->addWidget(slider);
	layout->addLayout(hlayout);
	
	QLabel *minus = new QLabel("-");
	minus->setAlignment(Qt::AlignHCenter);
	minus->setFont(f);
	layout->addWidget(minus);
	
	setLayout(layout);
	setFixedWidth(28);
	
	snd = new AmeSystemSound(this);
	snd->setEmbedSound(AmeSoundTheme::Popup, true);
	
	slider->setMinimum(0);
	slider->setMaximum(100);
	if (snd->isMuted()) {
		// TODO: update icon to muted
	} else {
		// TODO:
	}
	slider->setValue(snd->volume());

	connect(slider, SIGNAL(sliderReleased()), this, SLOT(onSliderRelease()));
	connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderMove(int)));
	connect(snd, SIGNAL(systemVolumeChanged(int)), this, SLOT(onSystemVolume(int)));

        timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateVolume()));
	changing = false;
}

void SliderWidget::setAutoRefresh(bool enable)
{
	if (enable) {		
		timer->start(100);
	} else {
		timer->stop();
	}
}

void SliderWidget::updateVolume()
{
	if (changing) // do not auto update when moving slider, it is done already
		return;
	int v = snd->volume();
	slider->setValue(v);
	emit volumeChanged(v);
}

void SliderWidget::onSliderRelease()
{
	if (playFeedback)
		snd->play();
}

void SliderWidget::onSliderMove(int v)
{
	changing = true;
	snd->setVolume(v);
	changing = false;
	emit volumeChanged(v);
}

void SliderWidget::onSystemVolume(int volume)
{
	if (!changing) {
		snd->setVolume(volume);
	}
}

SliderWidget::~SliderWidget()
{
}

VolumeCtrl::VolumeCtrl(Panel *p, QWidget *parent)
	: GenericButton(p, parent)
{
	can_Activated = false;
	setFixedWidth(28);
	setIconSize(QSize(19, 18));	
	volumeSlider = new SliderWidget(this);
	
	connect(volumeSlider, SIGNAL(volumeChanged(int)), this, SLOT(updateIcon(int)));
	updateIcon(volumeSlider->snd->volume());
}

VolumeCtrl::~VolumeCtrl()
{
}

void VolumeCtrl::activate()
{
	if (!activated) {
		panel->deactivateLast(this);
		volumeSlider->move(this->mapToGlobal(QPoint(0, height())));
		volumeSlider->updateVolume();
		volumeSlider->setAutoRefresh(true);
		volumeSlider->show();
		XRaiseWindow(display(), volumeSlider->winId());
		activated = true;
		panel->setCurrentWidget(this, volumeSlider->winId());
	}
}

void VolumeCtrl::deactivate()
{
	if (activated) {
		volumeSlider->setAutoRefresh(false);
		volumeSlider->hide();
		activated = false;
		panel->unfocusChilds();
		panel->clearCurrentWidget();
		m_Focus = false;
		update();
	}
}

void VolumeCtrl::updateIcon(int v)
{
	if (v >= 0 && v <= 10) {
		// volume0
		setImages(QPixmap(":/default/volume0.png"), QPixmap(":/default/volume0-active.png"));
	} else if (v > 10 && v <= 33) {
		// volume1
		setImages(QPixmap(":/default/volume1.png"), QPixmap(":/default/volume1-active.png"));
	} else if (v > 33 && v <= 75) {
		setImages(QPixmap(":/default/volume2.png"), QPixmap(":/default/volume2-active.png"));
		// volume2
	} else if (v > 76) {
		// volume3
		setImages(QPixmap(":/default/volume3.png"), QPixmap(":/default/volume3-active.png"));
	}
	update();
}

void VolumeCtrl::enableFeedback(bool enable)
{
	volumeSlider->playFeedback = enable;
}

/*
void VolumeCtrl::mousePressEvent(QMouseEvent *)
{
	panel->grabFocus();
	if (!activated) {
		activated = true;
		m_Focus = true;
	} else {
		activated = false;
		m_Focus = false;
	}
	panel->unfocusChilds();
	update();
	
}
*/

