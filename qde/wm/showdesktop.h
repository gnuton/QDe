#ifndef __DESKTOP_H
#define __DESKTOP_H

#include "button.h"

class Adx;
class Panel;

class ShowDesktop : public GenericButton
{

	Q_OBJECT

public:
	
	explicit ShowDesktop(Adx *a, Panel *p, QWidget *parent = 0);
	virtual ~ShowDesktop(void);

	virtual void deactivate();
	
	virtual void mousePressEvent(QMouseEvent *e);


protected:
	Adx *app;
	bool m_iconState;

	void iconify();
};

#endif

