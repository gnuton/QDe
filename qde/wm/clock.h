#ifndef __CLOCK_H
#define __CLOCK_H

#include "defs.h"

class ClockWidget : public QWidget
{
	Q_OBJECT 
public:
	ClockWidget(QWidget *parent = 0);
	virtual ~ClockWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:
	void paintEvent(QPaintEvent *event);

private:
	QTimer *m_Timer;
	QString m_Format;

};

#endif

