#ifndef __SINGLEAPP_H
#define __SINGLEAPP_H

#include <QtGui>

class SingleApp : public QApplication
{
	Q_OBJECT
public:
	SingleApp(int& argc, char** argv);
	~SingleApp();

	static bool isRunning();
protected:
	void init();
};

#endif
