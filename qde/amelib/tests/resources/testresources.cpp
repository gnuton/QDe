#include <QtDebug>
#include <QtCore/QDir>
#include <Resources>

int main(int argc, char **argv)
{
	qDebug("Standard dirs :");
	QString local = AmeResources::global()->stdDirs(PineResources::Local);
	QString desktop = AmeResources::global()->stdDirs(PineResources::Desktop);
	QString documents = AmeResources::global()->stdDirs(PineResources::Documents);
	QString autostart = AmeResources::global()->stdDirs(PineResources::Autostart);
	QString icons = AmeResources::global()->stdDirs(PineResources::IconTheme);
	
	
	qDebug() << "local dir = " << local;
	qDebug() << "Desktop = " << desktop;
	qDebug() << "Documents = " << documents;
	qDebug() << "Autostart = " << autostart;
	qDebug() << "Icon theme = " << icons;
	qDebug("end...");
	return 0;
}
