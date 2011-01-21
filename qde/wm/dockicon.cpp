//////////////////////////////////////////////////
//  File      : dockicon.h                      //
//  Originally written by: g_cigala@virgilio.it //
//  Modified by: ludmiloff@gmail.com            //
//  Copyright : GPL                             //
//////////////////////////////////////////////////

#include "dockicon.h"

DockIcon::DockIcon(Client *c, QWidget *parent) : QLabel(parent)
{
	client = c;
	type = DockIcon::Task;
	readSettings();
}

DockIcon::DockIcon(const QString &file, QWidget *parent) : QLabel(parent)
{
	client = NULL;
	type = DockIcon::Launcher;
        desktopFile = new AmeDesktopFile(file);
}

DockIcon::~DockIcon()
{
        delete desktopFile;
}


void DockIcon::readSettings()
{
	QSettings *stg = new QSettings();
	stg->beginGroup("Dockicon");
	titleColor = stg->value("title_color").toString();
	pix = QPixmap::fromImage(QImage(stg->value("d_icon_pix").toString()));
	stg->endGroup(); //Dockicon
}

void DockIcon::showClient()
{
	if (type == DockIcon::Task) {
		client->map();
		close();
	}
}

void DockIcon::updateSize(int w, int h)
{
	if (type == DockIcon::Task) {
		setFixedSize(w, h);
	} else if (type == DockIcon::Launcher) {
		setFixedSize(h, h);
	}
}

void DockIcon::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QColor(titleColor));
	painter.drawRoundRect(0, 0, width(), height(), 5, 20);
	painter.drawText(QRect(height()+3, 0, width(), height()), Qt::AlignVCenter, client->clientName); // dock text
	painter.drawPixmap(QRect(0, 0, width(), height()), pix, QRect(0, 0, pix.width(), pix.height()));// dock icon
	painter.drawPixmap(QRect(3, 3, height()-6, height()-6), client->appIcon,
		QRect(0, 0, client->appIcon.width(), client->appIcon.height()));// dock icon
   
}

void DockIcon::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		if (type == DockIcon::Task) {
			emit showIcon(this);
			client->map();
			close();
		}
	}

	if (event->button() == Qt::RightButton) {
		QMenu *menu = new QMenu(this);
		menu->addAction( QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton), "Close");
		menu->popup(event->globalPos());
		connect(menu, SIGNAL(triggered(QAction *)), this, SLOT(runMenu(QAction *)));
	}
}

void DockIcon::runMenu(QAction *)
{
	emit destroyIcon(this);
	client->destroyClient();
	close();
}
