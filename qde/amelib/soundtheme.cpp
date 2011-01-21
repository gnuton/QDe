#include <AmeDirs>
#include <SoundTheme>

AmeSoundThemeItem::AmeSoundThemeItem(const QString &aname, const QString &afileName, int id, bool aenabled)
{
	name = aname;
	fileName = afileName;
	soundId = id;
	enabled = aenabled;
}

AmeSoundThemeItem::~AmeSoundThemeItem()
{
}


AmeSoundTheme::AmeSoundTheme(QObject *parent)
	: QObject(parent)
{
	list = new SoundList;
	settings = new AmeSettings(AmeDirs::global()->stdDir(AmeDirs::Configs) + "/Sound", QSettings::IniFormat);
        readThemeSettings();
        initSounds();
}

AmeSoundTheme::~AmeSoundTheme()
{
	saveThemeSettings();
	delete list;
	// FIXME:
	settings->sync();
	delete settings;
}

AmeSoundThemeItem *AmeSoundTheme::getSound(int soundId)
{
	return list->value(soundId);
}

void AmeSoundTheme::setEnabled(int soundId, bool enabled)
{
	AmeSoundThemeItem *it;
	if ((it = list->value(soundId)) != NULL) {
		it->enabled = enabled;
		// save this permanently
		saveThemeSettings();
	}
}

bool AmeSoundTheme::setSound(int soundId, AmeSoundThemeItem *sound)
{
	if (soundId > AmeSoundTheme::LastSoundId) {
		list->insert(soundId, sound);
		return true;
	} else
		return false;
}

Sounds AmeSoundTheme::getAll() const
{
	return list->values();
}

void AmeSoundTheme::initSounds()
{
        addFile("Click.ogg", "Click", AmeSoundTheme::Click);
        addFile("Error1.ogg", "Application error", AmeSoundTheme::Error1);
        addFile("Error2.ogg", "System error", AmeSoundTheme::Error1);
        addFile("Clap.ogg", "Clap", AmeSoundTheme::Clap);
        addFile("Maximize.ogg", "Maximize", AmeSoundTheme::Maximize);
        addFile("Minimize.ogg", "Minimize", AmeSoundTheme::Minimize);
        addFile("Popup.ogg", "Notification popup", AmeSoundTheme::Popup);
        addFile("Question.ogg", "Question", AmeSoundTheme::Question);
        addFile("Information.ogg", "Information", AmeSoundTheme::Information);
        addFile("Restore_down.ogg", "Restore down", AmeSoundTheme::RestoreDown);
        addFile("Restore_up.ogg", "Restore up", AmeSoundTheme::RestoreUp);
        addFile("LockItem.ogg", "Lock/Unlock item", AmeSoundTheme::Lock);
        addFile("Shade.ogg", "Shade/Withdraw", AmeSoundTheme::Shade);
        // the setting may be changed, save'em now
        saveThemeSettings();
}

void AmeSoundTheme::addFile(const QString &file, const QString &name, int soundId)
{
        QString path(themeDir + "/" + file);
        if (QFile::exists(path)) {
                list->insert(soundId, new AmeSoundThemeItem(name, path, soundId));
        } else {
                // file not found: disable this sound
                AmeSoundThemeItem *it;
                if ((it = list->value(soundId)) != NULL) {
                        it->enabled = false;
                }
        }
}

void AmeSoundTheme::readThemeSettings()
{
        QDir d;
        themeName = settings->value("theme", QVariant("default")).toString();

        if (d.exists("/usr/share/ame/sounds/" + themeName)) {
                themeDir = "/usr/share/ame/sounds/" + themeName;
        }

        if (d.exists(AmeDirs::global()->stdDir(AmeDirs::SoundTheme) + themeName)) {
                themeDir = AmeDirs::global()->stdDir(AmeDirs::SoundTheme)  + themeName;
        }


	QStringList e = settings->listValue("sounds_enabled");
	if (e.isEmpty()) {
		// save defaults
		saveThemeSettings();
		return;
	}
	for (int i=0; i < e.size(); i++) {
		if (e.at(i) == "1")
			setEnabled(i, true);
		else
			setEnabled(i, false);
	}
}

void AmeSoundTheme::saveThemeSettings()
{
	QStringList e;
	Sounds lst = getAll();
	for (int i=0; i < lst.size(); i++) {
		e << (lst.at(i)->enabled ? "1" : "0");
	}
        settings->setValue("theme", themeName);
        settings->setListValue("sounds_enabled", e);
}

AME_GLOBAL_STATIC(AmeSoundTheme, gAmeSoundTheme)

AmeSoundTheme *AmeSoundTheme::global()
{
	return gAmeSoundTheme;
}
