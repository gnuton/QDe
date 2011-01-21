#ifndef __SIDEBAR_H
#define __SIDEBAR_H

#include <QWidget>
#include <NavBar>
#include <NavBarGroup>

class Sidebar : public AmeNavBar
{
	Q_OBJECT
public:
	Sidebar(QWidget *parent = 0);
	~Sidebar();

	void initPlaces();

	AmeNavBarGroup *devices;
	AmeNavBarGroup *places;

public slots:
	void onItem(AmeNavBarGroup*, AmeNavBarItem*);

signals:
	void newPath(const QString &name, const QString &path);
};

#endif
