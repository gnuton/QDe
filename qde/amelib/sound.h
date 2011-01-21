//////////////////////////////////////////////////////////
//  File      : sound.cpp				//
//  Written by: ludmiloff@gmail.com			//
//  Copyright : GPL2					//
//	Some parts of the implementation		//
//  are taken/inspired from Ogg123 code			//
//  THE Ogg123 SOURCE CODE IS (C) COPYRIGHT 2000-2001	//
//  by Stan Seibert <volsung@xiph.org> AND OTHER 	//
//  CONTRIBUTORS, http://www.xiph.org/			//
//							//
//  Sound volume mixer taken from QMMP project - 	//
//  http://qmmp.ylsoftware.com/				//
//							//
//  Some modifications & stripping made by me: 		//
//  <ludmiloff@gmail.com>				//
//////////////////////////////////////////////////////////

 
#ifndef __AMESOUND_H
#define __AMESOUND_H

#include <Global>
#include <QFile>
#include <QThread>
#include <QSettings>
#include <AmeDirs>

#include <alsa/asoundlib.h>

//Phonon
#include <phonon/MediaObject>
#include <phonon/AudioOutput>

class AME_EXPORT AmeSystemSound : public QThread
{
	Q_OBJECT
public:
	AmeSystemSound(bool withMixer = true, QObject *parent = 0);
	~AmeSystemSound();

	// Player API
	bool play(const QString &src);
	bool play();
	void stop();
	void pause();
	void resume();
	void setEmbedSound(int soundId, bool force=false);

	// Mixer API
	int volume();
	void setVolume(int);
	bool isMuted();
	bool mixerHasSwitch();
	void mute();

	// Mixer setup routines
	// public here for easy access outside AmeSystemSound
	int setupMixer(QString card, QString device);
	bool reinitMixer(QString card, QString device);
	void parseMixerName(char *str, char **name, int *index);
	int getMixer(snd_mixer_t **mixer, QString card);

	//
	void initialize();
	void uninitialize();
		
public slots:
	void stopPlaying();
	void onQuit();
	void onVolumeChanged(int);

signals:
	void stopped();
	void systemVolumeChanged(int);
	
protected:
	// Thread run
	virtual void run();

private:
	QString source;
	QFile *input;
	bool mInited;


	// Output cmd
	bool mPlay;
	bool mPause;
	bool mUserStop;


	// Alsa mixer
	bool useMixer;
	snd_mixer_elem_t* getMixerElem(snd_mixer_t *mixer, char *name, int index);
	snd_mixer_t *mixer;
	snd_mixer_elem_t *mixer_element;

        // Phonon
        Phonon::MediaObject *media;
        Phonon::AudioOutput *output;
};

#endif
