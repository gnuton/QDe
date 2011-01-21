#ifndef __QUIT_DIALOG_H
#define __QUIT_DIALOG_H

#include <QDialog>
#include "ui_frm_quit.h"

class QuitDlg : public QDialog
{
	Q_OBJECT

public:
	enum {
		fLogout,
		fReboot,
		fHalt,
		fSleep
	};
	QuitDlg(int func, QWidget *parent = 0);
	~QuitDlg(void);

public slots:
	void countDown();

private:
	Ui::quitDlg ui;
	int m_Counter;
	int m_Func;
	QTimer *m_Timer;

};

#endif

