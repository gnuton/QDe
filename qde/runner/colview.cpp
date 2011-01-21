#include "colview.h"

ColumnView::ColumnView(QWidget *parent) : QAbstractItemView(parent)
{
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
}

ColumnView::~ColumnView()
{
}


QModelIndex ColumnView::indexAt(const QPoint &point) const
{
	// TODO:
        return QModelIndex();
}

QRect ColumnView::visualRect(const QModelIndex &index) const
{
	return activeColumn()->visualRect(index);
}

//QAbstractItemView *ColView::createColumn(const QModelIndex & index)
//{
//	//qDebug() << "creating column";
//	QAbstractItemView *view = QColumnView::createColumn(index);
//	return view;
//}
