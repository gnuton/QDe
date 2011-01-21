//////////////////////////////////////////
//  File      : toolbar.h		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#ifndef __AMETOOLBAR_H
#define __AMETOOLBAR_H

#include <Global>
#include <QToolBar>
#include <RectButton>

class AME_EXPORT AmeToolBar : public QToolBar
{
	Q_OBJECT
public:
	AmeToolBar(QWidget *parent = 0);
	~AmeToolBar();

	void addSpace(int width);

        void addButton(AmeRectButton *button);
};

#endif
