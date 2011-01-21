#include <QtCore>
#include <QString>
#include <SoundTheme>
#include <Sound>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	app.setApplicationName("testsound.app");
	qDebug() << "begin";
	AmeSystemSound *snd = new AmeSystemSound();
	snd->setEmbedSound(AmeSoundTheme::Click);
	snd->playPhonon("");
	while (snd->isRunning()) {
	    qDebug() << "waiting";
	}
/*	
	Phonon::MediaObject *media = new Phonon::MediaObject();
	Phonon::AudioOutput *output = new Phonon::AudioOutput(Phonon::MusicCategory);
	Phonon::createPath(media, output);
	//media->setCurrentSource(QString(":/sounds/Click1.ogg"));
	media->enqueue(QString(":/sounds/Click1.ogg"));
	qDebug() << media->errorString();
	Phonon::MediaSource src = media->currentSource();
	qDebug() << src.deviceName() << src.fileName() << src.type();
	media->stop();
	media->play();
	qDebug() << media->errorString();
*/
	return app.exec();
}

