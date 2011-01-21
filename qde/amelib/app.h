#ifndef __AMEAPPLICATION_H
#define __AMEAPPLICATION_H

#include <Global>
#include <IconTheme>
#include <QtGui/QApplication>

class AME_EXPORT AmeApplication : public QApplication
{
public:
	explicit AmeApplication(int& argc, char** argv, bool single = false);
	virtual ~AmeApplication();

	bool isSingle;
	bool isRunning;

protected:
	AmeIconTheme* theme;

	virtual void init();

private:
	QObject obj;

};

#endif /* __AMEAPPLICATION_H */
