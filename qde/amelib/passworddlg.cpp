//////////////////////////////////////////
//  File      : passworddlg.cpp		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#include <PasswordDlg>
#include <SoundTheme>
#include <Sound>
#include "ui_password.h"

class AmePasswordDlg::PasswordDlgPrivate
{
public:
	PasswordDlgPrivate(AmePasswordDlg *qd)
		: qd(qd)
	{}

	void init();
	
	AmePasswordDlg *qd;
	Ui_passwordFrm ui;
	AmeSystemSound *snd;
};

void AmePasswordDlg::PasswordDlgPrivate::init()
{
	ui.setupUi(qd);
	snd = new AmeSystemSound(qd);
}


AmePasswordDlg::AmePasswordDlg(QWidget *parent)
	: QDialog(parent), d (new PasswordDlgPrivate(this))
{
	d->init();
	//connect(d->ui.usernameEdit, SIGNAL(returnPressed()), this, SLOT(accept()));
	//connect(d->ui.passwordEdit, SIGNAL(returnPressed()), this, SLOT(accept()));
}

AmePasswordDlg::~AmePasswordDlg()
{
	d->snd->stopPlaying();
	delete d->snd;
	delete d;
	d = NULL;
}

void AmePasswordDlg::playSoundWarning()
{
	d->snd->setEmbedSound(AmeSoundTheme::Error2);
	d->snd->play();
}

QString AmePasswordDlg::username() const
{
	return d->ui.usernameEdit->text();
}

QString AmePasswordDlg::password() const
{
	return d->ui.passwordEdit->text();
}

bool AmePasswordDlg::userHook()
{
	return true;
}

bool AmePasswordDlg::selectFirstEmptyField()
{
	if (d->ui.usernameEdit->text().isEmpty()) {
		d->ui.usernameEdit->setFocus();
		d->ui.usernameEdit->selectAll();
		return true;
	}
	if (d->ui.passwordEdit->text().isEmpty()) {
		d->ui.passwordEdit->setFocus();
		d->ui.passwordEdit->selectAll();
		return true;
	}
	return false;
}

bool AmePasswordDlg::check()
{
	return 
	   (!d->ui.usernameEdit->text().isEmpty() && 
		!d->ui.passwordEdit->text().isEmpty() &&
		userHook());
}

void AmePasswordDlg::accept()
{
	if (check()) 
	{
		QDialog::accept();
	} else {
		// TODO: password is wrong, sound error
		// assume passoword field is wrong
		selectFirstEmptyField();
	}
}

AmeSudoDlg::AmeSudoDlg(const QString &cmd, QWidget *parent)
	: AmePasswordDlg(parent)
{
	command = cmd;
	proc = new AmeUserProcess();
}

AmeSudoDlg::~AmeSudoDlg()
{
}

void AmeSudoDlg::accept()
{
	if (!selectFirstEmptyField()) {
		//qDebug() << "BEFORE ACCEPT";
		proc->setUser(username(), password());
		proc->exec(command, AmeUserProcess::WaitForExit);
		int status = proc->status();
		if (status == AmeUserProcess::Finished) {
			//qDebug() << "STATUS " << status; 
			QDialog::accept(); 
		} 
		if (status == AmeUserProcess::WrongPassword || status == AmeUserProcess::WrongCommand ||
			status == AmeUserProcess::Crashed) {
			qDebug() << "WRONG PASSWORD";
			// something goes wrong - play sound, if sound file given, warn the user
			playSoundWarning();
			d->ui.passwordEdit->selectAll();
			d->ui.passwordEdit->setFocus();
		}
	}
}
