#include <QtGui>
#include "quitdlg.h"

QuitDlg::QuitDlg(int func, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_Func = func;
	switch (m_Func) {
		case fLogout:
			ui.quitIcon->setPixmap(QPixmap(":/icons/logout.png"));
			ui.question->setText("Are you sure you want to quit all applications and log out now ?"); 
			ui.reminder->setText("If you do nothing, the system will log out automatically in 30 seconds.");
			ui.quitBtn->setText("Logout");
			break;			
		case fReboot:
			ui.quitIcon->setPixmap(QPixmap(":/icons/restart.png"));
			ui.question->setText("Are you sure you want to quit all applications and restart now ?"); 			
			ui.reminder->setText("If you do nothing, the system will restart automatically in 30 seconds.");
			ui.quitBtn->setText("Restart");			
			break;
		case fHalt:
			ui.quitIcon->setPixmap(QPixmap(":/icons/halt.png"));
			ui.question->setText("Are you sure you want to quit all applications and shutdown now ?"); 
			ui.reminder->setText("If you do nothing, the system will shutdown automatically in 30 seconds.");
			ui.quitBtn->setText("Shutdown");			
			break;			
	}
	m_Counter = 30;
	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(countDown()));
	m_Timer->start(1000);
}

void QuitDlg::countDown()
{
	m_Counter -= 1;
	switch (m_Func) {
		case fLogout:
			ui.reminder->setText(QString("If you do nothing, the system will log out automatically in %1 seconds.").arg(m_Counter));
			break;
		case fReboot:
			ui.reminder->setText(QString("If you do nothing, the system will restart automatically in %1 seconds.").arg(m_Counter));
			break;
		case fHalt:
			ui.reminder->setText(QString("If you do nothing, the system will shutdown automatically in %1 seconds.").arg(m_Counter));
			break;
	}
	if (m_Counter == 0) accept();
}


QuitDlg::~QuitDlg(void)
{
	m_Timer->stop();
	delete m_Timer;
}

