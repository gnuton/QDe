/* 
 *  This file is taken from Quartica project (MacNavBar).
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * 
 *	Some modifications made by me. <ludmiloff@gmail.com>
 *	- changed class name to fit Amelib namespace;
 *
 */

#ifndef __AMENAVBARGROUP_H
#define __AMENAVBARGROUP_H

// Qt4 Headers
#include <QWidget>

#include <Global>
#include <NavBarItem>

class AME_EXPORT AmeNavBarGroup : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(bool isExpanded READ isExpanded WRITE expand)

public:
	AmeNavBarGroup (QWidget *parent = 0);
	AmeNavBarGroup (const QString& title, QWidget *parent = 0);
	~AmeNavBarGroup();

	// Methods - Add Item
	void addItem (AmeNavBarItem *item);
	void addItem (AmeNavBarItem *item, int index);

	// Methods - Create and Add Item
	AmeNavBarItem *addItem (const QString &text, const QString &link);
	AmeNavBarItem *addItem (const QPixmap &icon, const QString &text, const QString &link);
	AmeNavBarItem *addItem (const QPixmap &icon, const QString &text, const QString &link, int index);

	// Methods
	bool containsItem (AmeNavBarItem *item);

	// GET Properties
	QString title (void) const;
	bool isExpanded (void) const;

	// SET Properties
	void setTitle (const QString& title);
	void setTitleColor (const QColor& color);

signals:
	void selected (AmeNavBarGroup *group, AmeNavBarItem *item);
	void expanded (AmeNavBarGroup *group);

public slots:
	void expand (bool expand);

private slots:
	void onItemSelected (AmeSelectableWidget *item);
	void onTitleClicked (void);

private:
	class Private;
	Private *d;
};

#endif

