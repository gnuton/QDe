//////////////////////////////////////////
//  File       : dockbar.cpp		//
//  Written by : g_cigala@virgilio.it	//
//  Modified by: ludmiloff@gmail.com	//
//  Copyright  : GPL2			//
//////////////////////////////////////////

#include <QUrl>
#include <Mime>
#include "dockbar.h"
#include "dockicon.h"
#include "adx.h"

Dockbar::Dockbar(Adx *a, QWidget *parent) : QLabel(parent)
{
	app = a;
	dockLayout = new QHBoxLayout();
	setLayout(dockLayout);
	dockLayout->setContentsMargins(0, 0, 0, 0);
	dockLayout->setSpacing(1);
	readSettings();
	setPixmap(QPixmap::fromImage(QImage(dockPix)));
	setScaledContents(true);
	
        timer = new QTimer(this);
        hideTimer = new QTimer(this);
	
	dockState = Dockbar::Normal;
	setAutoHide(autoHide);
        setAcceptDrops(true);

	// to store dockicons
	iconsList = new DockIconsList;

	// for dockicon on dockbar
	dockFrame = new QFrame();
	dockFrame->resize(QApplication::desktop()->width(), height()-1);
	iconLayout = new QHBoxLayout();
	dockFrame->setLayout(iconLayout);
	iconLayout->setAlignment(Qt::AlignLeft);
	iconLayout->setContentsMargins(0, 0, 0, 0);
	iconLayout->setSpacing(1);

	dockLayout->addWidget(dockFrame);
	setSizeFactor(dockSizeFactor, false);
	setWindowOpacity(0.7);
}

Dockbar::~Dockbar()
{
}

void Dockbar::readSettings()
{
	app->stg->beginGroup("Dockbar");
        dockPix = app->stg->value("dock_pix", QCoreApplication::applicationDirPath() + "/../share/themes/antico/default/dockbar.png").toString();
	dockSizeFactor = app->stg->value("dock_factor", 100).toInt();
	setAnimSpeed(app->stg->value("dock_anim_factor", 0).toInt());
	autoHide = app->stg->value("dock_autohide", false).toBool();
	app->stg->endGroup(); //Dockbar
}

void Dockbar::saveSettings()
{
	app->stg->beginGroup("Dockbar");
	app->stg->setValue("dock_autohide", autoHide);	
	app->stg->endGroup();
	app->stg->sync();
}

void Dockbar::setAnimSpeed(int factor)
{
	dockAnimFactor = factor;
	delay = 800 + dockAnimFactor * 1000;
	speed = 1 + 10 * dockAnimFactor;
}

void Dockbar::addClient(Client *client)
{
	DockIcon *dockIcon = new DockIcon(client);
	iconsList->append(dockIcon);
	updateSize();
	iconLayout->addWidget(dockIcon);
	connect(dockIcon, SIGNAL(showIcon(DockIcon *)), SLOT(removeIcon(DockIcon *))); // show iconize dockicon and update dockbar size
	connect(dockIcon, SIGNAL(destroyIcon(DockIcon *)), SLOT(removeIcon(DockIcon *))); // show iconize dockicon and update dockbar size
}

void Dockbar::removeIcon(DockIcon *d)
{
	iconsList->removeOne(d);
	app->onDockIconRemoved(d->client);
	updateSize();
}

bool Dockbar::removeClient(Client *client)
{
	DockIcon *d;
	foreach(d, *iconsList)
	{
		if (d->client == client) {
			iconsList->removeOne(d);
			d->close();
			updateSize();
			break;
		}			
	}
	return true; 
}

bool Dockbar::removeAll()
{
	DockIcon *d;
	foreach(d, *iconsList)
	{
		d->client->mapFast();
		iconsList->removeOne(d);
		d->close();
		updateSize();
	}
	return true; 		
}

void Dockbar::addLauncher(const QString &file)
{
}

void Dockbar::dragEnterEvent(QDragEnterEvent *event)
{
        if (event->mimeData()->hasUrls()) {
                event->acceptProposedAction();
        }
}

void Dockbar::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        foreach (QUrl url, event->mimeData()->urls()) {
                qDebug() << url.path();
                AmeMime mime;
                qDebug() << mime.fromFileName(url.path());
        }
    }
}


void Dockbar::setAutoHide(bool active)
{
	autoHide = active;
	// write settings
	saveSettings();
	
	if (autoHide) {
		connect(hideTimer, SIGNAL(timeout()), this, SLOT(hideShowDock()));
		hideTimer->start(80);
	} else {
		hideTimer->stop();
		disconnect(hideTimer, SIGNAL(timeout()), 0, 0);
		animateShow();
	}
}

// checkCursor : checks if the cursor is within dockbar area
// returns: true if yes, false if not
bool Dockbar::checkCursor()
{
	int dx = QCursor::pos().x();
	int dy = QCursor::pos().y();
	bool f1 = true;
	bool f2 = true;
	
	int w = app->desktop->width();
	
	if (dockState == Dockbar::Normal || dockState == Dockbar::Showing) {
		if (dx < x() || dx > x() + width())
			f1 = false;
		if (dy <= y() - 4)
			f2 = false;
	} else if (dockState == Dockbar::Hidden || dockState == Dockbar::Hiding) {
		if (dx < w / 3 || dx > w * 2 / 3)
			f1 = false;
		if (dy < app->desktop->height() + app->toppanel->height() - 16)
			f2 = false;
	}
	//qDebug() << "CHECK CURSOR :" << (f1 && f2);
	return (f1 &&  f2);
}

void Dockbar::hideShowDock()
{
	hideTimer->stop();
	hideTimer->start(delay);
	if (dockState == Dockbar::Normal && !checkCursor()) {
		animateHide();
	} else if (dockState == Dockbar::Hidden && checkCursor())
		animateShow();
}

void Dockbar::animateHide()
{
	if (checkCursor()) {
		// cursor is within dockbar area - cancel hiding
		return;
	}
	if (dockState == Dockbar::Normal) {
		connect(timer, SIGNAL(timeout()), this, SLOT(hide1Step()));
		dockState = Dockbar::Hiding;
		timer->start(speed);
	}
}

void Dockbar::hide1Step()
{
	if (checkCursor()) {
		// cursor is within dockbar area - cancel hiding
		timer->stop();
		disconnect(timer, SIGNAL(timeout()), 0, 0);
		dockState = Dockbar::Hidden;
		animateShow();
		return;
	}

	move(x(), y() + 1);
	if (y() == app->desktop->height() + app->toppanel->height()) {
		timer->stop();
		disconnect(timer, SIGNAL(timeout()), 0, 0);
		dockState = Dockbar::Hidden;
	}
}

void Dockbar::showFast()
{
	if (dockState == Dockbar::Normal)
		return;

	timer->stop();
	hideTimer->stop();
	move(x(), app->desktop->height() + app->toppanel->height() - height());

	autoHide = false;
	dockState = Dockbar::Normal;
	saveSettings();

}

void Dockbar::animateShow()
{
	if (dockState == Dockbar::Hidden) {
		connect(timer, SIGNAL(timeout()), this, SLOT(show1Step()));
		dockState = Dockbar::Showing;
		timer->start(speed);
	}
}

void Dockbar::show1Step()
{
	move(x(), y() - 1);
	if (y() == app->desktop->height() + app->toppanel->height() - height()) {
		timer->stop();
		disconnect(timer, SIGNAL(timeout()), 0, 0);
		dockState = Dockbar::Normal;
	}
}

int Dockbar::actualHeight()
{
	if (dockState == Dockbar::Normal)
		return height();
	else 
		return 0;
}

// 
//  dockbar::setSizeFactor
//  @param - factor (0,100) precent of screen width
// 
void Dockbar::setSizeFactor(int factor, bool doSave)
{
	dockSizeFactor = factor;
	if (doSave)
		saveSettings();
	
	//qDebug() << "RESIZING DOCK " << factor;
	int maxW = QApplication::desktop()->width();
	int w = (maxW * factor) / 100;
	int h = MAX_DOCK_HEIGHT * factor/100;
	if (h < 24) h = 24;
	//qDebug() << "DESIRED DOCK SIZE " << w << h;
	dockFrame->resize(w, h);
	resize(w, h);
	move((maxW - w)/2, QApplication::desktop()->height()-h); 
	//qDebug() << "ACTUAL SIZE " << width() << height();
	updateSize();
}

void Dockbar::updateSize(void)
{
	int dockLength;
	DockIcon *d;

	if (! iconsList->isEmpty()) {
		int num = iconsList->size();
		qDebug() << "dockicon num:" << num;
		dockLength = dockFrame->width()/num;

		if (dockLength >= dockFrame->width()/4) // max dockicon size = d_frame size/4
			dockLength = dockFrame->width()/4;

		qDebug() << "dockicon length:" << dockLength;

		for (int i = 0; i < num; i++) {
			d = iconsList->at(i);
			d->updateSize(dockLength, height()-5);
			iconLayout->addWidget(d);
		}
	}
}

