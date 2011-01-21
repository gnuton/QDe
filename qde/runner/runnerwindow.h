#ifndef __RUNNERWINDOW_H
#define __RUNNERWINDOW_H

#include <QtGui>
#include <Settings>
#include <ToolBar>
#include "sidebar.h"
#include "listview.h"
#include "colview.h"

class RunnerWindow : public QMainWindow
{
	Q_OBJECT
public:
	enum DisplayMode {Columns, Tree, Icons};

	RunnerWindow();
	~RunnerWindow();

	void createMenu();
	void createToolbar();
	void createStatusBar();

	void readSettings();
	void saveSettings();

public slots:
	void onPrev();
	void onNext();
	void onPath(const QString &name, const QString &path);

protected:
	void closeEvent(QCloseEvent *event);

private:
	AmeSettings *stg;

	QSplitter *splitter;
	QStackedWidget *stack;
	AmeNavBar *sidebar;
	AmeToolBar *toolbar;
	QStatusBar *status;
        AmeRectButton *prevBtn, *nextBtn,  *listBtn, *iconsBtn, *columnsBtn;

	int displayMode;
	QDirModel *dm;

	ColumnView *colView;
	ListView *listView;
};

#endif
