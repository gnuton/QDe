//////////////////////////////////////////
//  File       	: soundpref.cpp		//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL2			//
//////////////////////////////////////////

#include <AmeDirs>
#include <SoundTheme>
#include "soundpref.h"
#include <QtDBus>

QString hwDeviceInfo::fullName() const
{
	return QString("card : %1 - %2, device : %3 - %4").arg(card).arg(cardName).arg(device).arg(deviceName);
}

SoundPref::SoundPref(QWidget *parent)
	: PrefWidget(parent)
{
	ui.setupUi(this);

	AmeDirs *ame = AmeDirs::global();
	stg1 = new AmeSettings(ame->stdDir(AmeDirs::Configs) + "/Sound", QSettings::IniFormat);

	readSettings();

	// Sound effects tab
	ui.soundsLst->setColumnCount(2);
	ui.soundsLst->setColumnWidth(0, 420);

	QStringList hdr;
	hdr << "Name" << "Enabled";
	ui.soundsLst->setHeaderLabels(hdr);
	Sounds lst = AmeSoundTheme::global()->getAll();
	for (int i=0; i<lst.size(); i++) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui.soundsLst);
		it->setText(0, lst.at(i)->name);
		it->setText(1, "");
		if (lst.at(i)->enabled)
			it->setCheckState(1, Qt::Checked);
		else
			it->setCheckState(1, Qt::Unchecked);
		it->setData(1, Qt::UserRole, QVariant(lst.at(i)->soundId));
	}

	connect(ui.soundsLst, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onSoundItemChange(QTreeWidgetItem *, int)));
	connect(ui.soundsLst, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(onSoundItem(QTreeWidgetItem *, int)));

	ui.volumeSldr->setMinimum(0);
	ui.volumeSldr->setMaximum(100);
	connect(ui.volumeSldr, SIGNAL(sliderMoved(int)), this, SLOT(onSlider(int)));
	connect(ui.volumeSldr, SIGNAL(sliderReleased()), this, SLOT(onSliderRelease()));

	connect(ui.muteChk, SIGNAL(clicked()), this, SLOT(onMute()));
	connect(ui.sndVolFeedbackChk, SIGNAL(clicked()),this, SLOT(onVolumeFeedback()));
	connect(ui.showCtrlChk, SIGNAL(clicked()), this, SLOT(onShowHideVolumeCtrl()));

	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(checkVolume()));
	timer->start(100);

	// Output settings tab
	hwList = new hwDevices();
	getHwList(modePlayback);
	getMixerDevices(modePlayback);

	connect(ui.playbackMixerDeviceCmb, SIGNAL(activated(const QString &)), this, SLOT(onMixerDevice(const QString &)));

	refresh();

	moduleName = "Sound";
}

SoundPref::~SoundPref()
{
	delete hwList;
	timer->stop();
	saveSettings();
}

void SoundPref::refresh()
{
	ui.muteChk->setEnabled(snd->mixerHasSwitch());

	if (snd->isMuted()) {
		ui.muteChk->setChecked(true);
	} else {
		ui.muteChk->setChecked(false);
	}
	ui.volumeSldr->setValue(snd->volume());

}

void SoundPref::checkVolume()
{
	refresh();
}

void SoundPref::onSlider(int value)
{
	snd->setVolume(value);
}

void SoundPref::onSliderRelease()
{
	if (ui.sndVolFeedbackChk->isChecked()) {
		snd->setEmbedSound(AmeSoundTheme::Popup, true);
		snd->play();
	}
}

void SoundPref::onMute()
{
	snd->mute();
}

void SoundPref::onVolumeFeedback()
{
	// Notify AnticoDeluxe WM for playing volume feedback
	QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe",
		"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
		QDBusConnection::sessionBus(), this);
	if (!iface->isValid())
		qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
	else {
		iface->call("soundVolumeFeedback", ui.sndVolFeedbackChk->isChecked());
	}
	saveSettings();
}

void SoundPref::onShowHideVolumeCtrl()
{
	// Notify AnticoDeluxe WM for show / hide volume control
	QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe",
		"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
		QDBusConnection::sessionBus(), this);
	if (!iface->isValid())
		qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
	else {
		iface->call("showSoundVolumeCtrl", ui.showCtrlChk->isChecked());
	}
	saveSettings();
}

void SoundPref::onSoundItemChange(QTreeWidgetItem *it, int col)
{
	if (col == 1) {
		// sound item enabled/disabled by user
		AmeSoundTheme::global()->setEnabled(it->data(1, Qt::UserRole).toInt(), (it->checkState(col) == Qt::Checked));
	}
}

void SoundPref::onSoundItem(QTreeWidgetItem *it, int col) {
	Q_UNUSED(col);
	snd->setEmbedSound(it->data(1, Qt::UserRole).toInt(), true);
	snd->play("");
}

void SoundPref::onMixerDevice(const QString &device)
{
	mixerDevice = device;
	saveSettings();
	onChangeDevices();
	snd->reinitMixer(mixerCard, mixerDevice);
}

void SoundPref::onChangeDevices()
{
	// Notify AnticoDeluxe WM for changing sound devices
	QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe",
		"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
		QDBusConnection::sessionBus(), this);
	if (!iface->isValid())
		qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
	else {
		iface->call("changeSoundDevices", mixerCard, mixerDevice);
	}
}

void SoundPref::readSettings()
{	
	mixerCard = stg1->value("Mixer/mixer_card", "hw:0").toString();
	if (mixerCard.isEmpty()) {
		mixerCard = "hw:0";
		stg1->setValue("Mixer/mixer_card", mixerCard);
	}
	mixerDevice = stg1->value("Mixer/mixer_device", "Master").toString();
	if (mixerDevice.isEmpty()) {
		mixerDevice = "Master";
		stg1->setValue("Mixer/mixer_device", mixerDevice);
	}
	ui.sndVolFeedbackChk->setChecked(antico->value("Topbar/volume_feedback", true).toBool());
	ui.showCtrlChk->setChecked(antico->value("Topbar/show_volume_ctrl", true).toBool());
}

bool SoundPref::saveSettings()
{
	antico->beginGroup("Topbar");
	antico->setValue("volume_feedback", ui.sndVolFeedbackChk->isChecked());
	antico->setValue("show_volume_ctrl", ui.showCtrlChk->isChecked());
	antico->endGroup();
	antico->sync();

	stg1->setValue("Mixer/mixer_card", mixerCard);
	stg1->setValue("Mixer/mixer_device", mixerDevice);
	stg1->sync();

	stg->sync();
	return true;
}

void SoundPref::getHwList(int mode)
{
	snd_ctl_t *handle;
	int card, err;
	//int dev;

	snd_ctl_card_info_t *info;
	//snd_pcm_info_t *pcminfo;

	snd_ctl_card_info_alloca(&info);
	//snd_pcm_info_alloca(&pcminfo);

	card = -1;
	if (snd_card_next(&card) < 0 || card < 0) {
		qDebug() << "no soundcards found ...";
		return;
	}

	while (card >= 0) {
		QString hwName = QString("hw:%1").arg(card);
		if ((err = snd_ctl_open(&handle, strdup(hwName.toAscii().data()), 0)) < 0) {
			qDebug() << "Error while opening (" << card << "): " << card, snd_strerror(err);
			if (snd_card_next(&card) < 0) {
				// no more cards
				break;
			} else continue;

		}
		if ((err = snd_ctl_card_info(handle, info)) < 0) {
			qDebug() << "Error while getting hardware info (" << card << "):" << snd_strerror(err);
			snd_ctl_close(handle);
			if (snd_card_next(&card) < 0) {
				// no more cards
				break;
			} else continue;
		}

/*
		dev = -1;
		int indx = 0;
		while (1) {
			if (snd_ctl_pcm_next_device(handle, &dev) < 0)
				qDebug() << "no devices found ...";
			if (dev < 0)
				break;

			snd_pcm_info_set_device(pcminfo, dev);
			snd_pcm_info_set_subdevice(pcminfo, 0);
			snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_PLAYBACK);

			if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0) {
				if (err != -ENOENT)
					qDebug() << "error while getting digital audio info (" << card << "): "
						 << snd_strerror(err);
				continue;
			}

			hwDeviceInfo *hw = new hwDeviceInfo;
			hw->hwName = hwName;
			hw->card = card;
			hw->cardName = snd_ctl_card_info_get_name(info);
			hw->device = dev;
			hw->deviceName = snd_pcm_info_get_name(pcminfo);
			hwList->append(hw);
			indx ++;

			// append to selection list (ui)
			QListWidgetItem *it = new QListWidgetItem(hw->fullName(), ui.devicesLst);
			it->setData(Qt::UserRole, QVariant(indx));
			qDebug() << "inserting one card item";
		}
*/

		hwDeviceInfo *hw = new hwDeviceInfo;
		hw->card = card;
		hw->cardName = snd_ctl_card_info_get_name(info);

		hwList->insert(hwName, hw);

		QListWidgetItem *it = new QListWidgetItem(hw->cardName, ui.playbackDevicesLst);
		it->setData(Qt::UserRole, QVariant(hwName));

		if (hwName == mixerCard) {
			// select this item if it is default
			it->setSelected(true);
		}

		snd_ctl_close(handle);

		if (snd_card_next(&card) < 0) {
			// no more cards
			break;
		} else continue;
	}

	//snd_pcm_info_free(pcminfo);
	//snd_ctl_card_info_free(info);
}

void SoundPref::getMixerDevices(int mode)
{
	snd_mixer_t *mixer;
	QComboBox *cmb;
	if (mode == modePlayback)
		cmb = ui.playbackMixerDeviceCmb;
	else {
		// TODO : capture mixer devices
	}
	cmb->clear();

	int err;
	if ((err = snd->getMixer(&mixer, mixerCard)) < 0) {
		return;
	}

	snd_mixer_elem_t *elem;
	snd_mixer_selem_id_t *sid;
	snd_mixer_selem_id_alloca(&sid);
	int indx = 0;
	int current = -1;

	cmb->addItem("");

	for (elem = snd_mixer_first_elem(mixer); elem; elem = snd_mixer_elem_next(elem)) {
		snd_mixer_selem_get_id(elem, sid);
		if (!snd_mixer_selem_is_active(elem))
			continue;
		QString device = snd_mixer_selem_id_get_name(sid);
		if (mode == modePlayback) {
			if (snd_mixer_selem_has_playback_volume(elem))
				cmb->addItem(device);
		} else if (mode == modeCapture) {
			// TODO:
		} else
			continue;
		if (device == mixerDevice) {
			current = indx;
		}
		indx ++;
	}
	if (current >= 0)
		cmb->setCurrentIndex(current+1);
	
	snd_mixer_close(mixer);
}
