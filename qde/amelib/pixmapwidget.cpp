//////////////////////////////////////////
//  File      : pixmapwidget.cpp	//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QPaintEvent>

#include <PixmapWidget>

AmePixmapWidget::AmePixmapWidget(QWidget *parent)
	: QWidget(parent)
{
}

AmePixmapWidget::~AmePixmapWidget()
{
}

void AmePixmapWidget::setPixmap(const QPixmap pixmap)
{
	m_pixmap = pixmap;
	resize(m_pixmap.size());
	update();
}

void AmePixmapWidget::setBackground(const QPixmap pixmap)
{
	m_pixmap = pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	update();
}

void AmePixmapWidget::setSolidColor(const QColor color)
{
	QImage img(40, 40, QImage::Format_RGB32);
	QPainter painter(&img);
	painter.fillRect(0, 0, 40, 40, QBrush(color));
	setBackground(QPixmap::fromImage(img));
}

void AmePixmapWidget::paintEvent(QPaintEvent *)
{
        QPainter painter(this);
        painter.drawPixmap(0, 0, m_pixmap);
}

