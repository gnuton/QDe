/* 
 *  This file is taken from Quartica project (MacNavBar).
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * 
 *	Some modifications made by me. <ludmiloff@gmail.com>
 *	- changed class name to fit Amelib namespace;
 *
 */

#ifndef __AMENAVBAR_H
#define __AMENAVBAR_H

// Qt4 Headers
#include <QWidget>

#include <Global>
#include <NavBarGroup>

class AME_EXPORT AmeNavBar : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)
	Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

public:
	AmeNavBar (QWidget *parent = 0);
	~AmeNavBar();

	// Methods - Groups Related
	void addGroup (AmeNavBarGroup *group);
	AmeNavBarGroup *addGroup (const QString& title);

	bool containsGroup (AmeNavBarGroup *group);

	// Methods - Items Related
	void addItem (AmeNavBarGroup *group, AmeNavBarItem *item);
	AmeNavBarItem *addItem (AmeNavBarGroup *group, const QString &text, const QString &link);
	AmeNavBarItem *addItem (AmeNavBarGroup *group, const QPixmap &icon, const QString &text, const QString &link);

	// GET Properties
	QColor selectionColor (void) const;
	QColor backgroundColor (void) const;

	// SET Properties
	void setBackgroundColor (const QColor& bgColor);
	void setSelectionColor (const QColor& selectionColor);

protected:
	void paintEvent (QPaintEvent *event);

private slots:
	void onGroupExpanded (AmeNavBarGroup *group);
	void onItemSelected (AmeNavBarGroup *group, AmeNavBarItem *item);

private:
	class Private;
	Private *d;
};

#endif

