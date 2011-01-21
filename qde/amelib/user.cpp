//////////////////////////////////////////
//  File      : user.cpp		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#include <User>
#include <Utils>
#include <QtDebug>

class AmeUser::AmeUserPrivate
{
public:
	QString name;
	QString pwd;	
	QString shellCmd;
	bool valid;
	bool su;
	QIcon photo;

	AmeUserPrivate() {}
};

AmeUser::AmeUser(QObject *parent)
	: QObject(parent),
	  d (new AmeUserPrivate)
{
}

AmeUser::AmeUser(const QString &name, QObject * parent)
	: QObject(parent),
	  d (new AmeUserPrivate)
{
	if (isValid(name)) {	
		d->name = name;
		d->valid = true;
	} else
		d->valid = false;
	d->pwd = "";
}

AmeUser::AmeUser(const AmeUser &user)
	: QObject(), d(user.d)
{
}

AmeUser& AmeUser::operator =(const AmeUser &user)
{
	d = user.d;
	return *this;
}

QString AmeUser::shell() const
{
	return d->shellCmd;
}

QString AmeUser::loginName() const 
{
	return d->name;
}

QString AmeUser::homeDir() const
{
	return AmeUtils::homeDir(d->name);
}

QIcon AmeUser::loginPhoto() const
{
	return d->photo;
}

QString AmeUser::password() const
{
	return d->pwd;
}

void AmeUser::setLoginName(const QString &name)
{
	if (isValid(name)) {
		d->name = name;
		d->valid = true;
	} else
		d->valid = false;
}

void AmeUser::setPassword(const QString &pass)
{
	d->pwd = pass;
}

bool AmeUser::isValid(const QString &name)
{
	// TODO: finish this - real check if the username is valid
	return true;
}

AmeUser::~AmeUser()
{
	delete d;
	d = NULL;
}


// The code below is "stolen" from KdeSudo project,
// modified by me: Cvetoslav Ludmiloff <ludmiloff@gmail.com>
/***************************************************************************
                          kdesudo.cpp  -  the implementation of the 
                                          admin granting sudo widget
                             -------------------
    begin                : Sam Feb 15 15:42:12 CET 2003
    copyright            : (C) 2003 by Robert Gruber 
                                       <rgruber@users.sourceforge.net>
                           (C) 2007 by Martin Böhm <martin.bohm@kubuntu.org>
                                       Anthony Mercatante <tonio@kubuntu.org>
                                       Canonical Ltd (Jonathan Riddell 
                                                      <jriddell@ubuntu.com>)

****************************************************************************/

AmeUserProcess::AmeUserProcess(QObject *parent)
	: QObject(parent)
{
	user = new AmeUser;
	init();
}

AmeUserProcess::AmeUserProcess(const QString &name, QObject *parent)
	: QObject(parent)
{
	user = new AmeUser(name);
	init();
}

AmeUserProcess::AmeUserProcess(AmeUser *u, QObject *parent)
	: QObject(parent)
{
	user = u;
	init();
}


void AmeUserProcess::init()
{
	p = new QProcess;
	p1 = new QProcess;
	connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(parseOutput()));
	connect(p, SIGNAL(readyReadStandardError()), this, SLOT(parseOutput()));
	connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onExit()));
	p->setProcessChannelMode( QProcess::MergedChannels );
	m_Status = NotRunning;
}

AmeUserProcess::~AmeUserProcess()
{
}

void AmeUserProcess::setUser(const QString &name, const QString &pass)
{
	user->setLoginName(name);
	user->setPassword(pass);
}

bool AmeUserProcess::exec(const QString &cmd, int mode)
{
	if (user->password().isEmpty()) {
		p1->start("sudo -k");
		return true;
	} else { 
		p->start(QString("sudo -H -S -p upass -- %1").arg(cmd));
	}
	//p->waitForStarted();
	m_Status = Running;
	//qDebug() << "PROCESS STARTED " << user->password();
	if (mode == WaitForExit) {
		p->waitForFinished();
		//qDebug() << "FINISHED UNTIL WAIT";
	}
	return true;
}

void AmeUserProcess::parseOutput()
{
	//qDebug() << "PARSE OUTPUT " << m_Status;
	//if (m_Status != Running) return;

	QString strOut = p->readAllStandardOutput();
	//qDebug() << "OUT = " << strOut;
	if (strOut.contains("try again")) {
		//qDebug() << "Wrong password FINISHED";
		m_Status = WrongPassword;
	}

	if (strOut.contains("upass") || (strOut.contains("PIN (CHV2)"))) {
		if (m_Status == WrongPassword) {
			// TODO: Find a way to kill sudo for avoid 3times attempts
		}
		//qDebug() << "SEND PASSWORD :" << user->password();
		p->write(user->password().toLatin1()+"\n");
	} 

	else if (strOut.contains("command not found"))
		m_Status = WrongCommand;
	else if (strOut.contains("is not in the sudoers file"))
		m_Status = WrongPassword;	
	else if (strOut.contains("is not allowed to execute"))
		m_Status = WrongPassword;
	else if (strOut.contains("is not allowed to run sudo on"))
		m_Status = WrongPassword;
	else if (strOut.contains("may not run sudo on"))
		m_Status = WrongPassword;
}

void AmeUserProcess::onExit()
{
	if (m_Status == Running) {
		m_Status = Finished;
	}
	if (m_Status != WrongPassword) {
		emit processFinished(m_Status);
		p1->start("sudo -k");
	}
}

int AmeUserProcess::status()
{
	return m_Status;
}

