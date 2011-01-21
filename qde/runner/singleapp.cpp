#include "singleapp.h"

SingleApp::SingleApp(int& argc, char** argv)
	: QApplication(argc, argv)
{
}

SingleApp::~SingleApp()
{
}

bool SingleApp::isRunning()
{
	return false;
}

void SingleApp::init()
{
}
