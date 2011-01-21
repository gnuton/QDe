#ifndef __FOLDERVIEW_H
#define __FOLDERVIEW_H

#include <QtGui>
#include "listview.h"
#include "colview.h"

class FolderView : public QStackedWidget
{
	Q_OBJECT
public:

	enum ViewMode {Iconic, Columns, List};

	FolderView(int mode, QWidget *parent = 0);
	~FolderView();

public slots:
	void setViewMode(int mode);
	void setPath(const QString &path);

signals:
	void itemSelected();
	void itemOpenRequest();

protected:
	int viewMode;

	ListView *listView;
	ColumnView *colView;

};

#endif
