//////////////////////////////////////////////////////////
//  File      : sound.cpp				//
//  Written by: ludmiloff@gmail.com			//
//  Copyright : GPL2					//
//  Sound volume mixer taken fro QMMP project - 	//
//  http://qmmp.ylsoftware.com/				//
//							//
//  Some modifications & stripping made by me: 		//
//  <ludmiloff@gmail.com>				//
//////////////////////////////////////////////////////////

#include <Sound>
#include <SoundTheme>
#include <QString>


AmeSystemSound::AmeSystemSound(bool withMixer, QObject *parent)
	: QThread(parent),
	  mInited(false),
	  useMixer(withMixer)
{	
	initialize();
}

AmeSystemSound::~AmeSystemSound()
{
	uninitialize();
}

bool AmeSystemSound::play(const QString &src)
{
    if (!src.isEmpty())
            source = src;

    if (source.isEmpty())
            return false;

    media->setCurrentSource(Phonon::MediaSource(source));
    if (mInited) start();
    return mInited;
}


bool AmeSystemSound::play()
{
	return play("");
}


void AmeSystemSound::stop()
{
	mUserStop = true;
}

void AmeSystemSound::pause()
{
}

void AmeSystemSound::resume()
{
}

void AmeSystemSound::setEmbedSound(int soundId, bool force)
{
	AmeSoundThemeItem *sound = AmeSoundTheme::global()->getSound(soundId);
	if (sound != NULL) {
		if (sound->enabled || force)
			source = sound->fileName;
		else
			source = "";
	} else
		source = "";
        if (!QFile::exists(source))
                source = "";
}

int AmeSystemSound::volume()
{
	if (!mixer_element)
		return 0;
	
	long l, r;
	snd_mixer_handle_events(mixer);
	snd_mixer_selem_get_playback_volume(mixer_element, SND_MIXER_SCHN_FRONT_LEFT, &l);
	snd_mixer_selem_get_playback_volume(mixer_element, SND_MIXER_SCHN_FRONT_RIGHT, &r);

	return (l+r)/2;
}

void AmeSystemSound::setVolume(int v)
{
	if (!mixer_element)
		return;
	snd_mixer_selem_set_playback_volume(mixer_element, SND_MIXER_SCHN_FRONT_LEFT, v);
	snd_mixer_selem_set_playback_volume(mixer_element, SND_MIXER_SCHN_FRONT_RIGHT, v);
}


bool AmeSystemSound::isMuted()
{
	int sw;
	if (snd_mixer_selem_has_playback_switch(mixer_element)) {
		snd_mixer_selem_get_playback_switch(mixer_element, SND_MIXER_SCHN_FRONT_LEFT, &sw);
		return !sw;
	} else
		return false;
}

bool AmeSystemSound::mixerHasSwitch()
{
	if (snd_mixer_selem_has_playback_switch(mixer_element))
		return true;
	else
		return false;
}

void AmeSystemSound::mute()
{
	int sw;
	if (snd_mixer_selem_has_playback_switch(mixer_element)) {
		snd_mixer_selem_get_playback_switch(mixer_element, SND_MIXER_SCHN_FRONT_LEFT, &sw);
		snd_mixer_selem_set_playback_switch_all(mixer_element, !sw);
	}
}

void AmeSystemSound::onVolumeChanged(int volume)
{
	emit systemVolumeChanged(volume);
}


void AmeSystemSound::initialize()
{
	if (mInited)
		uninitialize();
	
	source = "";
	mixer = 0;
		
	mPause = false;
	mPlay = false;
	mUserStop = false;
	
	
	QSettings settings(AmeDirs::global()->stdDir(AmeDirs::Configs) + "/Sound", QSettings::IniFormat);
		
	if (useMixer) {
		QString mixerCard = settings.value("Mixer/mixer_card", "hw:0").toString();
		QString mixerDevice = settings.value("Mixer/mixer_device", "Master").toString();
		setupMixer(mixerCard, mixerDevice);
	}
	
        // Phonon initialization
        media = new Phonon::MediaObject(this);
        output = new Phonon::AudioOutput(Phonon::MusicCategory, this);
        Phonon::createPath(media, output);

	mPause = false;
	mPlay = false;
	mUserStop = false;
	mInited = true;
	return;
}

void AmeSystemSound::uninitialize()
{	
	
	if (mixer) {
		snd_mixer_close(mixer);	
		mixer = 0;
	}
	
	mPause = false;
	mPlay = false;
	mUserStop = false;
	
	mInited = false;
}

void AmeSystemSound::run()
{
        mPlay = true;
        media->play();
        mPlay = false;
}


void AmeSystemSound::stopPlaying()
{	
}


void AmeSystemSound::onQuit()
{
}


/* ****** MIXER PRIVATE ******* */

int AmeSystemSound::setupMixer(QString card, QString device)
{
    char *name;
    long int a, b;
    long alsa_min_vol = 0, alsa_max_vol = 100;
    int err, index;
    mixer_element = 0;

    qDebug("AmeSystemSound::SoundOutput: setupMixer()");

    if ((err = getMixer(&mixer, card)) < 0)
        return err;

    parseMixerName(device.toAscii().data(), &name, &index);

    mixer_element = getMixerElem(mixer, name, index);

    free(name);

    if (!mixer_element)
    {
        qWarning("SoundOutput: Failed to find mixer element");
        return -1;
    }

    /* This hack was copied from xmms.
     * Work around a bug in alsa-lib up to 1.0.0rc2 where the
     * new range don't take effect until the volume is changed.
     * This hack should be removed once we depend on Alsa 1.0.0.
     */
    snd_mixer_selem_get_playback_volume(mixer_element, SND_MIXER_SCHN_FRONT_LEFT, &a);
    snd_mixer_selem_get_playback_volume(mixer_element, SND_MIXER_SCHN_FRONT_RIGHT, &b);

    snd_mixer_selem_get_playback_volume_range(mixer_element,
            &alsa_min_vol, &alsa_max_vol);
    snd_mixer_selem_set_playback_volume_range(mixer_element, 0, 100);

    if (alsa_max_vol == 0)
    {
	mixer_element = NULL;
        return -1;
    }

    setVolume(a * 100 / alsa_max_vol);

    qDebug("SoundOutput: setupMixer() success");

    return 0;
}

bool AmeSystemSound::reinitMixer(QString card, QString device)
{
	if (!useMixer)
		return false;

	if (mixer) {
		snd_mixer_close(mixer);
		mixer = 0;
	}

	setupMixer(card, device);
	return true;
}

void AmeSystemSound::parseMixerName(char *str, char **name, int *index)
{
	char *end;
	
	while (isspace(*str))
		str++;
	
	if ((end = strchr(str, ',')) != NULL) {
		*name = strndup(str, end - str);
		end++;
		*index = atoi(end);
	} else {
		*name = strdup(str);
		*index = 0;
	}
}

snd_mixer_elem_t* AmeSystemSound::getMixerElem(snd_mixer_t *mixer, char *name, int index)
{
	snd_mixer_selem_id_t* selem_id;
	snd_mixer_elem_t* elem;
	snd_mixer_selem_id_alloca(&selem_id);
	
	if (index != -1)
		snd_mixer_selem_id_set_index(selem_id, index);
	if (name != NULL)
		snd_mixer_selem_id_set_name(selem_id, name);
	
	elem = snd_mixer_find_selem(mixer, selem_id);
	return elem;
}

int AmeSystemSound::getMixer(snd_mixer_t **mixer, QString card)
{
	char *dev;
	int err;
	
	dev = strdup(card.toAscii().data());
	if ((err = snd_mixer_open(mixer, 0)) < 0) {
		qWarning("SoundOutput: Failed to open empty mixer: %s", snd_strerror(-err));
		mixer = NULL;
		return -1;
	}
	
	if ((err = snd_mixer_attach(*mixer, dev)) < 0) {
		qWarning("SoundOutput: Attaching to mixer %s failed: %s", dev, snd_strerror(-err));
		return -1;
	}
    
	if ((err = snd_mixer_selem_register(*mixer, NULL, NULL)) < 0) {
		qWarning("SoundOutput: Failed to register mixer: %s", snd_strerror(-err));
		return -1;
	}
	
	if ((err = snd_mixer_load(*mixer)) < 0) {
		qWarning("SoundOutput: Failed to load mixer: %s", snd_strerror(-err));
		return -1;
	}
	
	free(dev);
	return (*mixer != NULL);
}
