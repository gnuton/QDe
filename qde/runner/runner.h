#ifndef __RUNNER_H
#define __RUNNER_H

#include <QtGui>
#include <AmeApp>

class RunnerApp : public AmeApplication
{
	Q_OBJECT
public:
	RunnerApp(int& argc, char** argv);
	~RunnerApp();
};

#endif
