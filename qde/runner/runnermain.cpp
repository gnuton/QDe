#include "runner.h"
#include "runnerwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("runner.app");
	RunnerWindow * w = new RunnerWindow();
	w->show();
	return app.exec();
}
