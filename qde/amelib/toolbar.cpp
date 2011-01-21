//////////////////////////////////////////
//  File      : toolbar.cpp		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#include <ToolBar>
#include <QWidget>
#include <QtDebug>

class SpaceItem : public QWidget
{
public:
	SpaceItem(int width, QWidget *parent = 0);
	~SpaceItem();
};

SpaceItem::SpaceItem(int width, QWidget *parent)
	: QWidget(parent)
{
	setFixedWidth(width);
}

SpaceItem::~SpaceItem()
{
}

AmeToolBar::AmeToolBar(QWidget *parent)
	: QToolBar(parent)
{
        setMovable(false);
        addSpace(10);
}

AmeToolBar::~AmeToolBar()
{
}

void AmeToolBar::addSpace(int width)
{
	addWidget(new SpaceItem(width));
}


void AmeToolBar::addButton(AmeRectButton *button)
{
        addWidget(button);
}
