#include <Utils>
#include <AmeDirs>
#include "sidebar.h"

Sidebar::Sidebar(QWidget *parent)
	: AmeNavBar(parent)
{
	initPlaces();
	setMinimumWidth(110);
	setMaximumWidth(300);
}

Sidebar::~Sidebar()
{
}

void Sidebar::initPlaces()
{
	devices = addGroup("DEVICES");
	//
	places = addGroup("PLACES");

	QString desktop = AmeDirs::global()->stdDir(AmeDirs::Desktop);
	if (AmeUtils::makeDir(desktop, 0700))
		addItem(places, QPixmap(":/icons/desktop.png"), "Desktop", desktop);

	addItem(places, QPixmap(":/icons/home.png"), "Home", AmeDirs::global()->stdDir(AmeDirs::Home));

	addItem(places, QPixmap(":/icons/applications.png"), "Applications", "/usr/share/applications/");


	connect(places, SIGNAL(selected(AmeNavBarGroup*,AmeNavBarItem*)),
		this, SLOT(onItem(AmeNavBarGroup*, AmeNavBarItem*)));
}

void Sidebar::onItem(AmeNavBarGroup*,AmeNavBarItem *it)
{
	qDebug() << "on item" << it->text() << it->link();
	emit newPath(it->text(), it->link());
}
