#ifndef __COLVIEW_H
#define __COLVIEW_H

#include <QtGui>
#include "listview.h"

class ColumnView : public QAbstractItemView
{
	Q_OBJECT
public:
	ColumnView(QWidget *parent = 0);
	~ColumnView();

	virtual QModelIndex indexAt(const QPoint &point) const;
	virtual QRect visualRect (const QModelIndex &index) const;

	ListView *activeColumn() const;

protected:

	QList<ListView* > columns;
	int current;

	//virtual QAbstractItemView * createColumn ( const QModelIndex & index );
};

inline ListView* ColumnView::activeColumn() const
{
	return columns[current];
}

#endif
