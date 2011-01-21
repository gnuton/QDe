#ifndef __DATETIME_H
#define __DATETIME_H

#include <QtGui>
#include "prefwidget.h"
#include "ui_datetime.h"

class DateTimePref : public PrefWidget
{
	Q_OBJECT
public:
	DateTimePref(QWidget *parent = 0);
	~DateTimePref();

	void enableCtrls(bool);
	virtual void readSettings();
	virtual bool saveSettings();
	
	bool setDateTime(const QString &datestring);

public slots:
	void updateTime(const QTime&);
	void updateDate(const QDate&);

	void onChangeDate(const QDate&);
	void onChangeTime(const QTime&);
	void onUnlock();
	void onSave();
	void onRevert();

private:
	Ui::datetimeFrm ui;
	bool locked;
	QDate oldDate;
	QTime oldTime;
	QString oldDateTime;
	QString uname, passwd;
};

#endif
