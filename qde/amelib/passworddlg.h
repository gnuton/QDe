//////////////////////////////////////////
//  File      : passworddlg.h			//
//  Written by: ludmiloff@gmail.com		//
//  Copyright : GPL2					//
//////////////////////////////////////////

#ifndef __PASSWORDDLG_H
#define __PASSWORDDLG_H

#include <Global>
#include <QtGui>
#include <User>

class AME_EXPORT AmePasswordDlg : public QDialog
{
	Q_OBJECT
public:
	AmePasswordDlg(QWidget *parent = 0);
	~AmePasswordDlg();

	QString username() const;
	QString password() const;
	virtual bool userHook();

public slots:
	void accept();
	void playSoundWarning();

signals:
	void helpRequest();


protected:
	virtual bool check();
	bool selectFirstEmptyField();
	//void playSoundWarning();

	class PasswordDlgPrivate;
	PasswordDlgPrivate *d;
};


class AmeSudoDlg : public AmePasswordDlg
{
	Q_OBJECT
public:
	AmeSudoDlg(const QString &cmd, QWidget *parent = 0);
	~AmeSudoDlg();

	//virtual bool userHook();

public slots:
	void accept();

protected:
	QString command;
	AmeUserProcess *proc;
};

#endif

