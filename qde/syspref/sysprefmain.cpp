#include <QApplication>
#include "sysprefwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("syspref.app");
	int module = 0;
	if (app.arguments().size() >= 3) {
		if (app.arguments().at(1) == "-w") {
			module = app.arguments().at(2).toInt();
		}
	}
	SysPref * sp = new SysPref(module);
	sp->show();
	return app.exec();
}

