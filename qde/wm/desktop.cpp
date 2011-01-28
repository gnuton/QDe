//////////////////////////////////////////////////
//  File      : desktop.cpp			//
//  Originally Written by: g_cigala@virgilio.it //
//  Modified by: ludmiloff@gmail.com		//
//  Copyright : GPL2				//
//////////////////////////////////////////////////

#include <adx.h>
#include "desktop.h"

Desktop::Desktop(Adx *a, QWidget *parent) : AmePixmapWidget(parent)
{
	app = a;
	layout = new QHBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
	setGeometry(QApplication::desktop()->geometry());
	QTimer::singleShot(0, this, SLOT(init()));
}

Desktop::~Desktop()
{
}

void Desktop::init()
{
        app->stg->beginGroup("Desktop");
	QString wall_pix = app->stg->value("wallpaper", "").toString();
	app->stg->endGroup();

	// TODO: check is wallpaper file exists, if not - fill background with default blue
        if (wall_pix.isEmpty() || wall_pix.isNull()) {
            setSolidColor(QColor(0x20, 0x60, 0xB0));
	} else
            setBackground(QPixmap::fromImage(QImage(wall_pix)));
}


void Desktop::loadWallpaper(const QString &path)
{
	setBackground(QPixmap::fromImage(QImage(path)));
}

void Desktop::runMenu(QAction *)
{
}
