#ifndef __PREVIEW_H
#define __PREVIEW_H

#include <QtGui>

class Preview : public QWidget
{
	Q_OBJECT
public:
	Preview(QWidget * parent = 0);
	~Preview();
};

#endif
