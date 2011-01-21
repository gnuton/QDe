#include <AmeDirs>
#include <Icon>
#include "runnerwindow.h"
#include "preview.h"

RunnerWindow::RunnerWindow()
{
	setWindowTitle("Runner");
	displayMode = RunnerWindow::Columns;

	AmeDirs *ame = AmeDirs::global();
	stg = new AmeSettings(ame->stdDir(AmeDirs::Configs) + "/Runner", QSettings::IniFormat);
	readSettings();

	splitter = new QSplitter(this);

	sidebar = new Sidebar(this);
	splitter->addWidget(sidebar);
	sidebar->show();

	stack = new QStackedWidget(this);
	splitter->addWidget(stack);

	QList <int> ww;
	ww << 140 << 600;
	splitter->setSizes(ww);

	dm = new QDirModel(this);
	dm->setSorting(QDir::DirsFirst);
	listView = new ListView(this);
	listView->setAlternatingRowColors(true);
	//colView->setPreviewWidget(new Preview());

	//ww.clear();
	//ww << 200 << 200 << 200 << 200 << 200 << 200 << 200 << 200
	//		<< 200 << 200 << 200 << 200 << 200 << 200 << 200 << 200;
	//colView->setColumnWidths(ww);
	listView->setModel(dm);
	listView->setRootIndex(dm->index(AmeDirs::global()->stdDir(AmeDirs::Home)));
	stack->addWidget(listView);

	connect(sidebar, SIGNAL(newPath(const QString, const QString)),
		this, SLOT(onPath(const QString, const QString)));

	splitter->setHandleWidth(1);

	createMenu();
	createToolbar();
	createStatusBar();

	setCentralWidget(splitter);
	//resize(QSize(795, 440));
}

RunnerWindow::~RunnerWindow()
{
}


void RunnerWindow::createMenu()
{
}

void RunnerWindow::createToolbar()
{
	toolbar = new AmeToolBar();
	addToolBar(toolbar);

        prevBtn = new AmeRectButton(AmeSystemIcon::GoPrevious, "Previous", this);
        connect(prevBtn, SIGNAL(clicked()), this, SLOT(onPrev()));

        nextBtn = new AmeRectButton(AmeSystemIcon::GoNext,"Next", this);
        connect(nextBtn, SIGNAL(clicked()), this, SLOT(onNext()));

        iconsBtn = new AmeRectButton(AmeSystemIcon::Icons, "Show as icons", this);
        listBtn = new AmeRectButton(AmeSystemIcon::List, "Show as list", this);
        columnsBtn = new AmeRectButton(AmeSystemIcon::Columns, "Show as list", this);


        toolbar->addWidget(prevBtn);
        toolbar->addWidget(nextBtn);
	toolbar->addSpace(60);
        toolbar->addWidget(iconsBtn);
        toolbar->addWidget(listBtn);
        toolbar->addWidget(columnsBtn);


}

void RunnerWindow::createStatusBar()
{
	status = new QStatusBar();
	setStatusBar(status);
}

void RunnerWindow::onPrev()
{
}

void RunnerWindow::onNext()
{
}

void RunnerWindow::onPath(const QString &name, const QString &path)
{
	setWindowTitle("Runner - " + name);
	listView->setRootIndex(dm->index(path));
}

void RunnerWindow::closeEvent(QCloseEvent *event)
 {
	saveSettings();
	event->accept();
 }

void RunnerWindow::readSettings()
{
     stg->beginGroup("MainWindow");
     resize(stg->value("size", QSize(795, 440)).toSize());
     stg->endGroup();
}

void RunnerWindow::saveSettings()
{
     stg->beginGroup("MainWindow");
     stg->setValue("size", size());
     stg->endGroup();

}
