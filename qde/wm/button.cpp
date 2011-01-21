#include "panel.h"
#include "button.h"

GenericButton::GenericButton(Panel *p, QWidget *parent)
	: 	QToolButton(parent),
		m_Focus(false), m_forceFocus(false),
		can_Activated(true), activated(false),
		m_type(APixmap)	
{
	panel = p;

	QFont f = QFont(font());
	f.setBold(true);
	f.setWeight(75);
	f.setPointSize(11);
	setFont(f);

	setFixedHeight(TOP_PANEL_HEIGHT);
}


void GenericButton::setImages(const QString &normal, const QString &active)
{
	setIcon(QPixmap::fromImage(QImage(normal).scaled(iconSize(), 
			Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

	focusPixmap = QPixmap::fromImage(QImage(active)).scaled(iconSize(), 
			Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void GenericButton::setImages(const QPixmap &normal, const QPixmap &active)
{
	setIcon(normal.scaled(iconSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	focusPixmap = QPixmap(active.scaled(iconSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
} 

GenericButton::~GenericButton(void)
{
}

void GenericButton::paintButton()
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect rect = painter.window();

	QColor base;
	if (m_Focus) {
		base = QColor(palette().highlight().color());
	} else {
		base = QColor(palette().button().color());
	}

	if (m_Focus) {
		QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));
		linearGrad.setColorAt(0, base.lighter(155));
		linearGrad.setColorAt(1, base.lighter(100));
		painter.fillRect(0, rect.top(), width(), height(), linearGrad);
	} else {
		QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));
		linearGrad.setColorAt(0, base);
		linearGrad.setColorAt(1, base.darker(200));	
		painter.fillRect(0, rect.top(), width(), height(), linearGrad);
	}
}

void GenericButton::paintText()
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect rect = painter.window();
	QRect offset(rect.left()+1, rect.top()+2, rect.width()-1, rect.height()-2);

	if (m_Focus)
		painter.setPen(palette().highlightedText().color());
	else {
		painter.setPen(QColor("#E7E7E7"));
		painter.drawText(offset, Qt::AlignCenter, this->text());
		painter.setPen(palette().text().color());
	}	
	painter.drawText(rect, Qt::AlignCenter, this->text());
}

void GenericButton::paintIcon()
{
	if (!icon().isNull() && !focusPixmap.isNull() && (m_type == APixmap)) {
		//qDebug() << "draw icon";
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		QRect rect = painter.window();
		
		int top = (rect.height() - iconSize().height()) / 2;
		int left = (rect.width() - iconSize().width()) / 2;

		QRect pixRect(left, top, iconSize().width(), iconSize().height());
		if (!m_Focus) {		
			QPixmap pix = icon().pixmap(iconSize());
			painter.drawPixmap(pixRect, pix);
		} else {
			painter.drawPixmap(pixRect, focusPixmap);
		}
		
	}
}

void GenericButton::paintEvent(QPaintEvent *)
{
	paintButton();
	paintIcon();
	if (!text().isNull())
		paintText();
}


void GenericButton::enterEvent(QEvent *) 
{
	if (panel->isFocused() && can_Activated) {
		activate();
		m_Focus = true;
		update();
	}	
}


void GenericButton::leaveEvent(QEvent *) 
{
	if (!activated) {
		m_Focus = false;
		update();
	}
}

void GenericButton::mousePressEvent(QMouseEvent *)
{
	panel->grabFocus();
	if (!activated) {
		activate();
		m_Focus = true;
	} else {
		deactivate();
		m_Focus = false;
	}
	update();
}

bool GenericButton::canActivated()
{
	return can_Activated;
}

void GenericButton::activate()
{
	if (menu() && !(activated) ) {
		panel->deactivateLast(this);
		menu()->popup(this->mapToGlobal(QPoint(0, height())));
		activated = true;
		panel->setCurrentWidget(this, menu()->winId());
	}
}

void GenericButton::deactivate()
{
	if (menu() && activated) {
		menu()->hide();
		activated = false;
		panel->clearCurrentWidget();
		m_Focus = false;
		update();
	}
}

void GenericButton::showMenu()
{
	//activate();
}
