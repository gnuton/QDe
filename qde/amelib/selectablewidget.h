/* 
 *  This file is taken from Quartica project (MacNavBar).
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * 
 *	Some modifications made by me. <ludmiloff@gmail.com>
 *	- changed class name to fit Amelib namespace;
 *
 */

#ifndef __AME_SELECTABLE_WIDGET_H_
#define __AME_SELECTABLE_WIDGET_H_

// Qt4 Headers
#include <QWidget>
#include <Global>

class AME_EXPORT AmeSelectableWidget : public QWidget {
	Q_OBJECT

	Q_PROPERTY(bool isSelected READ isSelected WRITE select)

public:
	AmeSelectableWidget (QWidget *parent = 0);
	~AmeSelectableWidget();

	bool isSelected (void) const;

signals:
	void selected (void);
	void selected (AmeSelectableWidget *widget);

public slots:
	void select (void);
	void select (bool isSelected);
	void unselect (void);

protected:
	void mouseReleaseEvent (QMouseEvent *event);

private:
	class Private;
	Private *d;
};

#endif


