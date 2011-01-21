/*
 *  This file is taken from Quartica project (MacNavBar).
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 *
 *	Some modifications made by me. <ludmiloff@gmail.com>
 *	- changed class name to fit Amelib namespace;
 *	- addes link member for storing shortcuts, links, paths etc.
 *
 */

#ifndef __AMENAVBARITEM_H
#define __AMENAVBARITEM_H

// Qt4 Headers
#include <QWidget>

#include <Global>
#include <SelectableWidget>

class AME_EXPORT AmeNavBarItem : public AmeSelectableWidget {
        Q_OBJECT

        Q_PROPERTY(QString text READ text WRITE setText)
        Q_PROPERTY(QPixmap icon READ icon WRITE setIcon)

public:
        AmeNavBarItem (QWidget *parent = 0);
	AmeNavBarItem (const QString &text, const QString &link, QWidget *parent = 0);
	AmeNavBarItem (const QPixmap &icon, const QString &text, const QString &link, QWidget *parent = 0);
        ~AmeNavBarItem();

        // Methods
        void addSpacing (int size);
        void addWidget (QWidget *widget, int stretch = 0);

        void insertSpacing (int index, int size);
        void insertWidget (int index, QWidget *widget, int stretch = 0);

        // GET Properties
	QString text(void) const;
	QString link(void) const;
        const QPixmap *icon (void) const;

        // SET Properties
	void setLink(const QString &link);
	void setIcon (const QPixmap &icon);
	void setText (const QString &text);
	void setTextColor (const QColor &color);

private:
        class Private;
        Private *d;
};

#endif


