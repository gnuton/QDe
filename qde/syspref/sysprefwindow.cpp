#include "sysprefwindow.h"
#include "prefwidget.h"
#include "appearance.h"
#include "wallpaper.h"
#include "dock.h"
#include "datetime.h"
#include "soundpref.h"
#include "intnl.h"
#include "dbusadapt.h"
#include <RectButton>

SysPref::SysPref(int module)
{
	setWindowTitle("System Preferences");
	setWindowIcon(QIcon(":/icons/object/system-preferences.png"));
	stack = new QStackedWidget;
	personal = new AmeStaticIconList("Personal", AmeStaticIconList::Odd);
	hardware = new AmeStaticIconList("Hardware", AmeStaticIconList::Even);
	system = new AmeStaticIconList("System", AmeStaticIconList::Odd);

	browser = new Browser;
	stack->addWidget(browser);
	createToolbar();

	setupLaunchers();
	setCentralWidget(stack);
	prevWidget = NULL;
	nextWidget = NULL;
	
	WMCtrl = new DBusAdaptor(this, &obj);
	QDBusConnection::sessionBus().registerObject("/", &obj);
	if (!QDBusConnection::sessionBus().registerService("org.freedesktop.AnticoPref")) {
		qDebug() << "UNABLE TO REGISTER DBUS SERVICE";
		exit(1);
	}
	
	resize(QSize(600, 350));
	
	switch (module) {
		case 1:
			onAppearance();
			break;
		case 2:
			onWallpaper();
			break;
		case 3:
			onDock();
			break;
		case 4:
			onDateTime();
			break;
		case 5:
			onIntnl();
			break;
	}
}

SysPref::~SysPref()
{
}

void SysPref::remoteFunction(int fEnumerator, int param)
{
	if (stack->currentIndex() == 0) return;
	PrefWidget *widget = (PrefWidget*) stack->widget(1);
	widget->execFunction(fEnumerator, param); 
}

void SysPref::onShowMain()
{
	if (stack->currentIndex() == 0) return;
	PrefWidget *widget = (PrefWidget*) stack->widget(1);
	widget->aboutToClose();
	stack->removeWidget(stack->widget(1));
	nextWidget = widget;
	discardPrevious();
	stack->setCurrentIndex(0);
	resize(QSize(600, 350));
	setWindowTitle("System Preferences");
}

void SysPref::onShowPrev()
{
	if (prevWidget != NULL) {
	} else
		onShowMain();
}

void SysPref::onShowNext()
{
	if (nextWidget != NULL) {
		stack->addWidget(nextWidget);
		nextWidget->show();
		stack->setCurrentIndex(1);
		discardPrevious();
		setWindowTitle(nextWidget->moduleName);
		nextWidget = NULL;
	}
}

void SysPref::createToolbar()
{
	toolbar = new AmeToolBar();
	addToolBar(toolbar);
        prev = new AmeRectButton(AmeSystemIcon::GoPrevious, "Previous section");
        connect(prev, SIGNAL(clicked()), this, SLOT(onShowPrev()));

        next = new AmeRectButton(AmeSystemIcon::GoNext,"Next section");
        connect(next, SIGNAL(clicked()), this, SLOT(onShowNext()));

        showAll = new AmeRectButton("Show All", "Show all sections", this);
        connect(showAll, SIGNAL(clicked()), this, SLOT(onShowMain()));


        toolbar->addButton(prev);
        toolbar->addButton(next);
        toolbar->addSpace(8);
        toolbar->addButton(showAll);

}

void SysPref::onWallpaper()
{
	Wallpaper *frm;
	if (nextWidget != NULL && nextWidget->moduleName == "Desktop Wallpaper") {
		frm = (Wallpaper *) nextWidget;
	} else {
		frm = new Wallpaper();
	}
	stack->addWidget(frm);
	frm->show();
	stack->setCurrentIndex(1);
	discardPrevious();
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);
}

void SysPref::onAppearance()
{
	Appearance *frm;
	if (nextWidget != NULL && nextWidget->moduleName == "Appearance") {
		frm = (Appearance *) nextWidget;
	} else {
		frm = new Appearance();
	}
	stack->addWidget(frm);
	frm->show();
	stack->setCurrentIndex(1);
	discardPrevious();
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);	
}

void SysPref::onDock()
{
	DockPref *frm;
	if (nextWidget != NULL && nextWidget->moduleName == "Dock") {
		frm = (DockPref *) nextWidget;
	} else {
		frm = new DockPref();
	}
	stack->addWidget(frm);
	frm->show();
	stack->setCurrentIndex(1);
	discardPrevious();
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);	
}

void SysPref::onDateTime()
{
	DateTimePref *frm;
	if (nextWidget != NULL && nextWidget->moduleName == "Date & Time") {
		frm = (DateTimePref *) nextWidget;
	} else {
		frm = new DateTimePref();
	}
	stack->addWidget(frm);
	frm->show();
	stack->setCurrentIndex(1);
	discardPrevious();
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);	
}

void SysPref::onSound()
{
	SoundPref *frm;
	if (nextWidget != NULL && nextWidget->moduleName == "Sound") {
		frm = (SoundPref*) nextWidget;
	} else {
		frm = new SoundPref();
	}
	stack->addWidget(frm);
	frm->refresh();
	frm->show();
	stack->setCurrentIndex(1);
	discardPrevious();
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);		
}

void SysPref::onIntnl()
{
        IntnlPref *frm;
        if (nextWidget != NULL && nextWidget->moduleName == "Language settings") {
                frm = (IntnlPref*) nextWidget;
        } else {
                frm = new IntnlPref();
        }
        stack->addWidget(frm);
        frm->show();
        stack->setCurrentIndex(1);
        discardPrevious();
        nextWidget = NULL;
        setWindowTitle(frm->moduleName);
}


void SysPref::addModule()
{
}

void SysPref::discardPrevious()
{
	if (prevWidget != NULL) {
		delete prevWidget;
		prevWidget = NULL;
	}
}

void SysPref::discardNext()
{
	if (nextWidget != NULL) {
		delete nextWidget;
		nextWidget = NULL;
	}
}

void SysPref::setupLaunchers()
{
	// Personal section
	AmeIconLink *appearance = new AmeIconLink("Appearance", ":/icons/object/appearance.png", "appearance", "");
	connect(appearance, SIGNAL(clicked()), this, SLOT(onAppearance()));

	AmeIconLink *desktop = new AmeIconLink("Desktop\nWallpaper", ":/icons/object/desktop.png", "desktop", "");
	connect(desktop, SIGNAL(clicked()), this, SLOT(onWallpaper()));

	//AmeIconLink *screensaver = new AmeIconLink("Screen\nSaver", path+"screensaver.png", "screensaver", "");
	
	AmeIconLink *dock = new AmeIconLink("Dock", ":/icons/object/dock.png", "dock", "");
        connect(dock, SIGNAL(clicked()), this, SLOT(onDock()));

        AmeIconLink *intnl = new AmeIconLink("Language\nsettings", ":/icons/object/language.png", "intnl", "");
        connect(intnl, SIGNAL(clicked()), this, SLOT(onIntnl()));


	personal->addIcon(appearance);
	personal->addIcon(desktop);
	//personal->addIcon(screensaver);
	personal->addIcon(dock);
        personal->addIcon(intnl);
	
	browser->addSection(personal);

	// Hardware section
	AmeIconLink *asound = new AmeIconLink("Sound", ":/icons/object/sound.png", "sound", "");
	hardware->addIcon(asound);
	connect(asound, SIGNAL(clicked()), this, SLOT(onSound()));
	
	browser->addSection(hardware);

	// System section
	AmeIconLink *aclock = new AmeIconLink("Date &\nTime", ":/icons/object/date-time.png", "datetime", "");
	system->addIcon(aclock);
	connect(aclock, SIGNAL(clicked()), this, SLOT(onDateTime()));

	browser->addSection(system);
	
	browser->addStretch();
}

void SysPref::closeEvent(QCloseEvent *event)
{
	//writeSettings();
	event->accept();
}
