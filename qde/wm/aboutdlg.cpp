#include "aboutdlg.h"
#include <QtGui>
#include <QFile>
#include <sys/sysinfo.h>

AboutDlg::AboutDlg(int major, int minor, const QString &mod, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.versionLbl->setText(QString("version %1.%2-%3").arg(major).arg(minor).arg(mod));
	getHWInfo();
}


void AboutDlg::getHWInfo()
{

	QFile file("/proc/cpuinfo");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "CANNOT OPEN /PROC/CPUINFO FOR READING PROCESSOR INFO";
		return;
	}

	QMap <QString, QString> map;
	bool f = false;

	while (true) {
		QString line = file.readLine();
		if (line.isEmpty()) break;
		QStringList data = (line).split(':');
		
		if (data.size() == 2) {
                        map.insert(data[0].simplified(), data[1].simplified());
			f = true;
		}
	}
	file.close();

	if (true) {
		ui.vendorLbl->setText(map.value("vendor_id"));
		ui.modelLbl->setText(map.value("model name"));
		ui.speedLbl->setText(map.value("cpu MHz") + " MHz");
	} else
		qDebug() << "NO PROCESSOR INFO AVAILABLE";

	struct sysinfo memory;
	sysinfo(&memory);

        uint in_mb = memory.totalram * memory.mem_unit / 1048576; // in Mega bytes
	float in_gb;
	if (in_mb >= 1024) { 
                in_gb = in_mb / 1024.0;
		ui.memoryLbl->setText(QString("%1 GB").arg(in_gb, 0, 'f', 2));
	} else {
		ui.memoryLbl->setText(QString("%1 MB").arg(in_mb));
	}
        //qDebug() << in_mb << in_gb << memory.mem_unit * memory.totalram;
	
}
