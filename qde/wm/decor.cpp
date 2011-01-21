//////////////////////////////////////////
//  File       	: decor.cpp		//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL2			//
//////////////////////////////////////////

#include "decor.h"
#include "atoms.h"

Border::Border(QWidget *parent) : QLabel(parent)
{
}

Border::~Border()
{
}

void Border::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		emit bMouseRightPress();
	} else if (event->button() == Qt::LeftButton) {
		emit bMouseLeftPress();
	} else if (event->button() == Qt::MidButton) {
		emit bMouseMidPress();
	}
}

void Border::mouseReleaseEvent(QMouseEvent *)
{
	emit bMouseRelease();
}

void Border::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton) { // to block right button mouse move
        emit bMouseMove(event);
	}
}

WinButton::WinButton(int buttonType, QWidget *parent)
	: QLabel(parent),
	  m_buttonType(buttonType)
{
	setFixedHeight(19);
	setFixedWidth(20);

	state = oldState = normalState;
	QString pathActive, pathNormal;
	switch (m_buttonType) {
		case CloseButton:
			pathActive = ":/default/close-btn-active.png";
			pathNormal = ":/default/close-btn.png";
			break;
		case MaximizeButton:
			pathActive = ":/default/maximize-btn-active.png";
			pathNormal = ":/default/maximize-btn.png";
			break;
		case MinimizeButton:
			pathActive = ":/default/minimize-btn-active.png";
			pathNormal = ":/default/minimize-btn.png";
			break;
	}
	normal = new QPixmap(pathNormal);
	active = new QPixmap(pathActive);
	inactive = new QPixmap(":/default/win-btn-inactive.png");
	update();
}

WinButton::~WinButton()
{
	delete active;
	delete normal;
	delete inactive;
}

void WinButton::changeState(int s)
{
	state = s;
	oldState = state;
	update();
}

void WinButton::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect rect = painter.window();
	
	// icon size is QSize(18, 17)
	int top = (rect.height() - 17) / 2;
	int left = (rect.width() - 18) / 2;

	QRect pixRect(left, top, 18, 17);

	if (state == normalState) {
		painter.drawPixmap(pixRect, *normal);
	} else if (state == activeState) {
		painter.drawPixmap(pixRect, *active);
	} else if (state == inactiveState) {
		painter.drawPixmap(pixRect, *inactive);
	}	
}

void WinButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		emit wMouseRightPress();
	} else if (event->button() == Qt::LeftButton) {
		emit wMouseLeftPress();
	} else if (event->button() == Qt::MidButton) {
		emit wMouseMidPress();
	}
}

void WinButton::enterEvent(QEvent *) 
{
	if (state != activeState) {
		oldState = state;
		state = activeState;
		update();
	}	
}

void WinButton::leaveEvent(QEvent *) 
{
	if (state != oldState) {
		state = oldState;
		update();
	}	
}

GenericBar::GenericBar(QWidget *parent)
	: QWidget(parent)
{
	borderState = borderActive;
	setFixedHeight(8);
	resize(width(), 8);
	changeCursor = true;
	setMouseTracking(true);
	cursorPos = GenericBar::NonePos;
	posLocked = false;

        top = QColor(157, 157, 157);
        bottom = QColor(150, 150, 150);
}

GenericBar::~GenericBar()
{
}

void GenericBar::changeState(int state)
{
	borderState = state;
	update();
}

void GenericBar::mousePressEvent(QMouseEvent *event)
{		
    if(event->button() == Qt::RightButton)
    {
        emit tMouseRightPress();
    }
    else if(event->button() == Qt::LeftButton)
    {
        emit tMouseLeftPress(event);
    }
    else if(event->button() == Qt::MidButton)
    {
        emit tMouseMidPress();
    }
	posLocked = true;
}

void GenericBar::mouseMoveEvent(QMouseEvent *event)
{
	if (changeCursor && !posLocked) {
		if (event->x() >= width() - 12) {
			setCursor(QCursor(Qt::SizeFDiagCursor));
			cursorPos = GenericBar::RightPos;
		} else {
			setCursor(QCursor(Qt::ArrowCursor));
			cursorPos = GenericBar::NonePos;
		}
	}
    if(event->buttons() == Qt::LeftButton) // to block right button mouse move
    {
        emit tMouseMove(event);
    }
}

void GenericBar::mouseReleaseEvent(QMouseEvent *)
{
        emit tMouseRelease();
                posLocked = false;
}

void GenericBar::paintEvent(QPaintEvent *)
{
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
    
	QRect rect = painter.window();

        if (borderState == borderActive) {
                // TODO:
        }

        int y1 = rect.top() + 1;
        int h1 = height() - 1;

        QLinearGradient linearGrad(QPointF(0, y1), QPointF(0, h1));
        linearGrad.setColorAt(0, top);
        linearGrad.setColorAt(1, bottom);
        painter.fillRect(0, y1, width(), h1, linearGrad);
}


Titlebar::Titlebar(const QPixmap &_icon, const QString &_title, int _role, QWidget *parent)
	: GenericBar(parent)
{

        top = QColor(197, 197, 197);
        bottom = QColor(178, 178, 178);
        topline = QColor(220, 220, 220, 150);

	icon = _icon;
	title = _title;
	role = _role;
	changeCursor = false;
	setMouseTracking(false);
	//borderState = borderActive;
	
	QFont f = QFont(font());
	f.setBold(true);
	f.setWeight(75);
	f.setPointSize(10);
	setFont(f);
	
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	if (role != Atoms::Dialog) {
		closeBtn = new WinButton(WinButton::CloseButton);
		layout->addWidget(closeBtn);
		closeBtn->show();
		//layout->addSpacing(1);

		minBtn = new WinButton(WinButton::MinimizeButton);
		layout->addWidget(minBtn);
		minBtn->show();
		//layout->addSpacing(1);

		maxBtn = new WinButton(WinButton::MaximizeButton);
		layout->addWidget(maxBtn);
		maxBtn->show();
		//layout->addSpacing(1);
	} else {
		closeBtn = NULL;
		minBtn = NULL;
		maxBtn = NULL;
	}
	layout->addStretch();
	setLayout(layout);

	setFixedHeight(TITLEBAR_HEIGHT);
	resize(width(), TITLEBAR_HEIGHT);
}

Titlebar::~Titlebar()
{
}

int Titlebar::getPreferedHeight(int role)
{
	// TODO:
	Q_UNUSED(role);
	return TITLEBAR_HEIGHT;
}

void Titlebar::changeState(int state)
{
	borderState = state;
	if (role != Atoms::Dialog) {
		if (state == borderInactive) {
			closeBtn->changeState(WinButton::inactiveState);
			minBtn->changeState(WinButton::inactiveState);
			maxBtn->changeState(WinButton::inactiveState);
		} else if (state == borderActive) {
			closeBtn->changeState(WinButton::normalState);
			minBtn->changeState(WinButton::normalState);
			maxBtn->changeState(WinButton::normalState);
		}
	}
	update();
}

void Titlebar::setTitle(const QString &_title)
{
	title = _title;
	update();
}

void Titlebar::paintEvent(QPaintEvent *event)
{
        GenericBar::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(topline);
        painter.drawLine(0, 0, width(), 0);

	QColor base;
	base = palette().button().color();
	if (borderState == borderActive)
		base = base.darker(140);

	painter.setPen(base.darker(250));// text color

	int h = height() + 4;
	if (icon.isNull()) {
		h = 0;
	}
	int w = (width() - fontMetrics().width(title) - h) / 2;

	painter.drawPixmap(QRect(w, 2, height()-4, height()-4), icon, 
                QRect(0, 0, icon.width(), icon.height()));//icon

        painter.drawText(QRect(w + h, 0, width(), height()),
		Qt::AlignVCenter, title); //text    
}

/*
void Titlebar::mousePressEvent(QMouseEvent *event)
{		
    if(event->button() == Qt::RightButton)
    {
        emit tMouseRightPress();
    }
    else if(event->button() == Qt::LeftButton)
    {
        emit tMouseLeftPress(event);
    }
    else if(event->button() == Qt::MidButton)
    {
        emit tMouseMidPress();
    }
}

void Titlebar::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) // to block right button mouse move
    {
        emit tMouseMove(event);
    }
}

void Titlebar::mouseReleaseEvent(QMouseEvent *)
{
    emit tMouseRelease();
}
*/

void Titlebar::mouseDoubleClickEvent(QMouseEvent *)
{
	emit tMouseDblClick();
}
