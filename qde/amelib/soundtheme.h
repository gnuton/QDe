#ifndef SOUNDTHEME_H
#define SOUNDTHEME_H

#include <Global>
#include <Settings>
#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class AME_EXPORT AmeSoundThemeItem
{
public:
	AmeSoundThemeItem(const QString &aname, const QString &afileName, int id = 0, bool aenabled = true);
	~AmeSoundThemeItem();

	QString name;
	QString fileName;
	bool enabled;
	int soundId;
};

typedef QMap<int, AmeSoundThemeItem *> SoundList;
typedef QList<AmeSoundThemeItem *> Sounds;

class AME_EXPORT AmeSoundTheme : public QObject
{
public:

	enum SystemSounds {
		Click, Error1, Error2, Clap,
		Maximize, Minimize, Popup,
                Question, Information,
		RestoreDown, RestoreUp,
		Lock, Shade,
		//
		LastSoundId
	};

	AmeSoundTheme(QObject *parent = 0);
	~AmeSoundTheme();

	AmeSoundThemeItem *getSound(int soundId);
	void setEnabled(int soundId, bool);
	bool setSound(int soundId, AmeSoundThemeItem *sound);
	Sounds getAll() const;

	static AmeSoundTheme *global();

private:
	SoundList *list;
	AmeSettings *settings;
        QString themeName, themeDir;

        void initSounds();
        void addFile(const QString &file, const QString &name, int soundId);
	void readThemeSettings();
	void saveThemeSettings();
};

#endif // SOUNDTHEME_H
