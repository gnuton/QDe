#include <SoundTheme>
#include "prefwidget.h"

PrefWidget::PrefWidget(QWidget *parent)
	: QWidget(parent)
{
	state = Normal;
	moduleName = "";

	// System preferences settings
	stg = new AmeSettings(AmeDirs::global()->stdDir(AmeDirs::Configs) + "/SystemPreferences", QSettings::IniFormat);

	// read AnticoDeluxe settings
	antico = new AmeSettings(AmeDirs::global()->stdDir(AmeDirs::Configs) + "/AnticoDeluxe", QSettings::IniFormat);

	snd = new AmeSystemSound(this);
	connect(this, SIGNAL(moduleIsUnlocked(bool)), this, SLOT(onUnlock(bool)));
}

PrefWidget::~PrefWidget()
{
	//snd->quit();
	delete snd;
}

void PrefWidget::execFunction(int fEnumerator, int param)
{
	// do nothing here
}

void PrefWidget::refresh()
{
}

bool PrefWidget::aboutToClose()
{
	saveSettings();
	return true;
}

void PrefWidget::onUnlock(bool)
{
	snd->setEmbedSound(AmeSoundTheme::Lock);
	snd->play();
}
