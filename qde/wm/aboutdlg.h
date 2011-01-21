#ifndef __ABOUT_DIALOG_H
#define __ABOUT_DIALOG_H

#include <QDialog>
#include "ui_frm_about.h"

class AboutDlg : public QDialog
{
public:
	AboutDlg(int major, int minor, const QString &mod, QWidget *parent = 0);

	void getHWInfo();

private:
	Ui::aboutDlg ui;
};

#endif

