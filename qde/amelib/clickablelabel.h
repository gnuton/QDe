/* 
 *  This file is taken from Quartica project (MacNavBar).
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * 
 *	Some modifications made by me. <ludmiloff@gmail.com>
 *	- changed class name to fit Amelib namespace;
 *
 */

#ifndef __AMECLICKABLELABEL_H
#define __AMECLICKABLELABEL_H

// Qt4 Headers
#include <QLabel>

#include <Global>

class AME_EXPORT AmeClickableLabel : public QLabel {
	Q_OBJECT

public:
	AmeClickableLabel (QWidget *parent = 0, Qt::WindowFlags f = 0);
	AmeClickableLabel (const QString& text, QWidget *parent = 0, Qt::WindowFlags f = 0);
	~AmeClickableLabel();

signals:
	void clicked (void);
	void clicked (AmeClickableLabel *label);

protected:
	void mouseReleaseEvent (QMouseEvent *event);
};

#endif

