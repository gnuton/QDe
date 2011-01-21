//////////////////////////////////////////
//  File      : pixmapwidget.h			//
//  Written by: ludmiloff@gmail.com		//
//  Copyright : GPL2					//
//////////////////////////////////////////

#ifndef __AMEPIXMAPWIDGET_H
#define __AMEPIXMAPWIDGET_H

#include <QWidget>

class QPixmap;
class QColor;

class AmePixmapWidget : public QWidget
{
	Q_OBJECT

public:
	AmePixmapWidget(QWidget *parent = 0);
	virtual ~AmePixmapWidget();

	virtual void setPixmap(const QPixmap);
	virtual void setBackground(const QPixmap);
	virtual void setSolidColor(const QColor);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QPixmap m_pixmap;	
};

#endif

