#include "folderview.h"

FolderView::FolderView(int mode, QWidget *parent)
	: QStackedWidget(parent)
{
	listView = new ListView();
	addWidget(listView);
        //colView = new ColumnView();
	//addWidget(colView);
	setViewMode(mode);
}

FolderView::~FolderView()
{
}

void FolderView::setViewMode(int mode)
{
	if (viewMode == mode) return;
	viewMode = mode;

	if (viewMode == FolderView::List) {
		setCurrentIndex(0);
		listView->setViewMode(QListView::ListMode);
	} else if (viewMode == FolderView::Iconic) {
		setCurrentIndex(0);
		listView->setViewMode(QListView::IconMode);
	} else if (viewMode == FolderView::Columns) {
		setCurrentIndex(1);
	}
}


void FolderView::setPath(const QString &path)
{

}
