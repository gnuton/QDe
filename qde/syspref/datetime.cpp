#include <PasswordDlg>
#include <Sound>
#include "datetime.h"

DateTimePref::DateTimePref(QWidget *parent)
	: PrefWidget(parent)
{
	ui.setupUi(this);
	readSettings();
	locked = TRUE;
	enableCtrls(false);
	oldDateTime = "";
	uname = "";
	passwd = "";
	moduleName = "Date & Time";

	oldDate = ui.dateEdit->date();
	oldTime = ui.timeEdit->time();

	oldDateTime = oldDate.toString("MMdd") + oldTime.toString("HHmm");

	connect(ui.unlockBtn, SIGNAL(clicked()), this, SLOT(onUnlock()));
	connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(onSave()));
	connect(ui.revertBtn, SIGNAL(clicked()), this, SLOT(onRevert()));
	connect(ui.timeEdit, SIGNAL(timeChanged(const QTime)), this, SLOT(onChangeTime(const QTime)));
	connect(ui.dateEdit, SIGNAL(dateChanged(const QDate)), this, SLOT(onChangeDate(const QDate)));
	
	connect(ui.aclock, SIGNAL(minutePassed(const QTime)), this, SLOT(updateTime(const QTime)));
	connect(ui.calendar, SIGNAL(clicked(const QDate)), this, SLOT(updateDate(const QDate)));
}

DateTimePref::~DateTimePref()
{
}

void DateTimePref::updateTime(const QTime &time)
{
	qDebug() << "time from clock widget";
	disconnect(ui.timeEdit, SIGNAL(timeChanged(const QTime)), 0, 0);
	ui.timeEdit->setTime(time);
	connect(ui.timeEdit, SIGNAL(timeChanged(const QTime)), this, SLOT(onChangeTime(const QTime)));
}

void DateTimePref::updateDate(const QDate &date)
{
	qDebug() << "date from calendar widget";
	disconnect(ui.dateEdit, SIGNAL(dateChanged(const QDate)), 0, 0);
	ui.dateEdit->setDate(date);
	connect(ui.dateEdit, SIGNAL(dateChanged(const QDate)), this, SLOT(onChangeDate(const QDate)));
}

void DateTimePref::onChangeDate(const QDate &date)
{
	qDebug() << "date from date edit";
	ui.calendar->setSelectedDate(date);
	ui.revertBtn->setEnabled(true);
}

void DateTimePref::onChangeTime(const QTime &time)
{
	qDebug() << "time from time edit";
	ui.aclock->setTime(time);
	ui.revertBtn->setEnabled(true);
}

void DateTimePref::onSave()
{
	QString datestring = ui.dateEdit->date().toString("MMdd") +
				ui.timeEdit->time().toString("HHmm") + ui.dateEdit->date().toString("yyyy");

	qDebug() << "DATE TIME" << datestring;
	if (setDateTime(datestring)) {
		ui.aclock->setShowCurrentDateTime(true);
		ui.revertBtn->setEnabled(true);
	}
}

void DateTimePref::onRevert()
{
	if (setDateTime(oldDateTime)) {
		ui.dateEdit->setDate(oldDate);
		ui.timeEdit->setTime(oldTime);
		ui.revertBtn->setEnabled(false);
		ui.aclock->setShowCurrentDateTime(true);
	}
}

bool DateTimePref::setDateTime(const QString &datestring)
{
	AmeUserProcess *p = new AmeUserProcess();
	p->setUser(uname, passwd);
	p->exec(QString("date %1").arg(datestring), AmeUserProcess::WaitForExit);
	qDebug() << "process status = " << p->status();
	// TODO: check process result
	return true;
}

void DateTimePref::enableCtrls(bool enable)
{
	ui.dateEdit->setEnabled(enable);
	ui.timeEdit->setEnabled(enable);
	ui.calendar->setEnabled(enable);
	if (enable) {
		ui.revertBtn->show();
		ui.revertBtn->setEnabled(false);
		ui.saveBtn->show();
		ui.lockLbl->setText("Click here to lock and prevent unwanted changes");
	} else {
		ui.revertBtn->hide();
		ui.saveBtn->hide();
		ui.lockLbl->setText("Click here to unlock and allow changes");
	}
}

void DateTimePref::onUnlock()
{
	if (locked) {
		// clear sudo flag
		AmeUserProcess *p = new AmeUserProcess;
		p->exec("");
		
		ui.unlockBtn->setChecked(false);		
		AmeSudoDlg *dlg = new AmeSudoDlg("true", this);
		
		if (dlg->exec() == QDialog::Accepted) {
			emit moduleIsUnlocked(true);
			locked = false;
			uname = dlg->username();
			passwd = dlg->password();
			enableCtrls(!locked);
			ui.unlockBtn->setIcon(QIcon(":/icons/tool/unlock.png"));
			ui.unlockBtn->setChecked(true);
		} else {
			qDebug() << "REJECTED";
		}
	} else {
		emit moduleIsUnlocked(false);
		ui.unlockBtn->setIcon(QIcon(":/icons/tool/lock.png"));
		ui.unlockBtn->setChecked(false);
		locked = true;
		
		QDateTime t = QDateTime::currentDateTime();
		
		ui.dateEdit->setDate(t.date());
		ui.timeEdit->setTime(t.time());
		ui.revertBtn->setEnabled(false);
		ui.aclock->setShowCurrentDateTime(true);

		uname = "";
		passwd = "";
		enableCtrls(!locked);
	}
}

void DateTimePref::readSettings()
{
	ui.dateEdit->setDate(QDate::currentDate());
	ui.timeEdit->setTime(QTime::currentTime());
}

bool DateTimePref::saveSettings()
{
	return true;
}
