#ifndef __LISTCOLUMN_H
#define __LISTCOLUMN_H

#include <QObject>
#include <QListView>

class ColumnView;

class ListView : public QListView
{
	Q_OBJECT
public:
	explicit ListView(QWidget *parent = 0);
	~ListView();

protected:
	ColumnView *colView;
};

#endif
