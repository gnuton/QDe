#include "app.h"

AmeApplication::AmeApplication(int& argc, char** argv, bool single)
	: QApplication(argc, argv),
	  isSingle(false),
	  isRunning(false)
{
	init();
	//theme = new AmeIconTheme(QString("default"), this);
}

AmeApplication::~AmeApplication()
{
}
  
void AmeApplication::init()
{
}
