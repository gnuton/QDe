#include "clock.h"

ClockWidget::ClockWidget(QWidget *parent)
	: QWidget(parent)
{
	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(update()));
	m_Timer->start(1000);

	QFont f = QFont(font());
	f.setBold(true);
	f.setWeight(75);
	f.setPointSize(11);
	setFont(f);

	m_Format = "hh:mm, MMM d, ddd";
	
	// Paint initialy
	update();
}


ClockWidget::~ClockWidget()
{
	m_Timer->stop();
	delete m_Timer;
}

QSize ClockWidget::minimumSizeHint() const 
{
	return QSize(20, 20);
}

QSize ClockWidget::sizeHint() const 
{
	return QSize(fontMetrics().width(m_Format)+10, 20);
}


void ClockWidget::paintEvent(QPaintEvent *)
{
	QDateTime current = QDateTime::currentDateTime();
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect rect = painter.window();
	QRect offset(rect.left()+1, rect.top()+2, rect.width()-1, rect.height()-2);
	
	// paint
	painter.setPen(QColor("#E7E7E7"));
	painter.drawText(offset, Qt::AlignLeft | Qt::AlignVCenter, current.toString(m_Format));
	painter.setPen(palette().text().color());	
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, current.toString(m_Format));
}

