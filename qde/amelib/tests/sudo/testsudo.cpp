#include <QtCore>
#include <QString>
#include <User>
#include "testsudo.h"

AProcess::AProcess(const QString &name, QObject *parent)
	: QObject(parent)
{
	user = new AmeUser(name);
	p = new QProcess;
	connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(parseOutput()));
	connect(p, SIGNAL(readyReadStandardError()), this, SLOT(parseOutput()));
	connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onExit()));
	connect(p, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onError(QProcess::ProcessError)));
	m_Status = NotRunning;
}

void AProcess::init()
{
}

void AProcess::onError(QProcess::ProcessError err)
{
	qDebug() << "error " << err;
}

AProcess::~AProcess(void)
{
}

bool AProcess::sudoExec(const QString &pwd, const QString &cmd)
{
	user->setPassword(pwd);

	p->setProcessChannelMode( QProcess::MergedChannels );

	if (pwd.isEmpty()) {
		p->start("sudo", QStringList() << "-k");
	} else { 
		qDebug() << "STARTING " << cmd ;
		p->start("sudo", QStringList() << "-H" << "-S" << "-p" << "passprompt" << "--" << cmd);

	}
	//p->waitForStarted();
	p->waitForFinished();
	//qDebug() << "NEW STATE " << p->state();
	if (m_Status != WrongPassword) 
	m_Status = NotRunning;
	return true;
}

void AProcess::parseOutput()
{
	QString strOut = p->readAllStandardOutput();
	qDebug() << "OUT: " << strOut;

	if (strOut.contains("try again")) {
		p->kill();
		p->write("\n");
		m_Status = WrongPassword;
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

	else if (strOut.contains("passprompt") || (strOut.contains("PIN (CHV2)"))) {
		qDebug() << "witing password";
		p->write(user->password().toLatin1()+"\n");
	} else {
		//fprintf(stdout, strOut.toLatin1().constData());
	}
}

void AProcess::onExit()
{
	qDebug() << "exit";
	user->setPassword("");
	if (m_Status == Running) {
	p->start("sudo", QStringList() << "-k");
	//p->waitForFinished(); 
	qDebug() << "clearing";
	} else if (m_Status == WrongPassword) {
		qDebug() << "wron password";
	} else 	
	m_Status = NotRunning;
}

int AProcess::status()
{
	return m_Status;
}


int main(int argc, char **argv)
{
	qDebug() << "begin";	
	AProcess *p = new AProcess("cl");
	p->sudoExec("12396", "date");
	int status = p->status();
	int i = 0;
	while (status == AProcess::Running) {
		// do nothing, waiting for process to exit
		status = p->status();
		//qDebug() << "status = " << status;
		i ++;
	}

	if (status == AProcess::WrongPassword) {
		qDebug() << "wrong password";
	} else if (status == AProcess::NotRunning) {
		qDebug() << "not running (assume done)";
	} else
		qDebug() << "other state";

	delete p;
	return 0;
}

