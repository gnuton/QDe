#ifndef __BROWSER_H
#define __BROWSER_H

#include <QtGui>
#include <StaticIconList>

class Browser : public QWidget
{
public:
	Browser(QWidget *parent = 0);
	~Browser();
	void addSection(AmeStaticIconList *section);
	void addStretch();

private:
	QVBoxLayout *layout;
};

#endif
