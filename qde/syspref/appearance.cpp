//////////////////////////////////////////
//  File       	: appearance.cpp	//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL2			//
//////////////////////////////////////////

#include <QtDBus>
#include "appearance.h"

Appearance::Appearance(QWidget *parent)
	: PrefWidget(parent)
{
	ui.setupUi(this);
	readSettings();
	setupStyles();
	setupColors();
	moduleName = "Appearance";
	
	QColor current = ui.colorCb->itemData(lastColorIndx, Qt::DecorationRole).value<QColor>();
	if (current != lastColor) {
		ui.colorCb->insertItem(7, "Custom");
		ui.colorCb->setItemData(7, current, Qt::DecorationRole);
		lastColor = current;
		lastColorIndx = 8;
	} else 
		ui.colorCb->setCurrentIndex(lastColorIndx);
	
	ui.minimizeDblClickChk->setChecked(dblClickMinimize);

	// Fixing this application color, seems buggy
	QPalette palette = QApplication::palette();
	palette.setColor(QPalette::Highlight, lastColor);
	QApplication::setPalette(palette);

	connect(ui.colorCb, SIGNAL(activated(int)), this, SLOT(onChangeColor(int)));
	connect(ui.styleCb, SIGNAL(activated(int)), this, SLOT(onChangeStyle(int)));
	
	connect(ui.minimizeDblClickChk, SIGNAL(toggled(bool)), this, SLOT(onDblClickOption(bool)));
}

Appearance::~Appearance()
{
}

void Appearance::onDblClickOption(bool active)
{
	dblClickMinimize = active;
	antico->beginGroup("Appearance");
	antico->setValue("minimize_dbl_click", dblClickMinimize);
	antico->endGroup();
	antico->sync();
	// Notify AnticoDeluxe WM about DblClick on titlebar function
	QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe", 
			"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
			QDBusConnection::sessionBus(), this);
	if (!iface->isValid())
		qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
	else
		iface->call("minimizeDblClick", dblClickMinimize);

}

void Appearance::onChangeColor(int indx)
{
	if (indx != lastColorIndx) {
		lastColor = ui.colorCb->itemData(indx, Qt::DecorationRole).value<QColor>();
		QStringList pal = rebuildPalette(lastColor);

		qts->beginGroup(QLatin1String("Qt"));
		qts->setValue(QLatin1String("Palette/active"), pal);
		qts->endGroup();
		qts->sync();
		lastColorIndx = indx;
		applyChanges();

		antico->beginGroup("Appearance");
		antico->setValue("last_highlight_indx", lastColorIndx);
		antico->setValue("last_highlight", lastColor.name());
		antico->endGroup();
		antico->sync();

		// Notify AnticoDeluxe WM about changing highlight color,
		// as it seems qt_x11_apply_settings_in_all_apps() does not affect Antico Deluxe WM
		QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe", 
				"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
				QDBusConnection::sessionBus(), this);
		if (!iface->isValid())
			qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
		else
			iface->call("changeHighlight", lastColor.red(), lastColor.green(), lastColor.blue());
		
		// Fixing this application color, seems buggy
		QPalette palette = QApplication::palette();
		palette.setColor(QPalette::Highlight, lastColor);
		QApplication::setPalette(palette);

	}
}

#ifdef Q_WS_X11
extern void qt_x11_apply_settings_in_all_apps();
#endif // Q_WS_X11
void Appearance::applyChanges()
{
#if defined(Q_WS_X11)
	qt_x11_apply_settings_in_all_apps();
#endif // Q_WS_X11
}

void Appearance::onChangeStyle(int indx)
{
	if (indx != lastStyleIndx) {
		qts->beginGroup(QLatin1String("Qt"));
		qts->setValue(QLatin1String("style"), ui.styleCb->itemText(indx));
		qts->endGroup();
		qts->sync();
		lastStyleIndx = indx;
		applyChanges();
		// Notify AnticoDeluxe WM about changing style,
		// as it seems qt_x11_apply_settings_in_all_apps() does not affect Antico Deluxe WM
		QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe", 
				"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
				QDBusConnection::sessionBus(), this);
		if (!iface->isValid())
			qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
		else
			iface->call("changeStyle", ui.styleCb->itemText(indx));
	}	
}


void Appearance::readSettings()
{
	antico->beginGroup("Appearance");
	lastColorIndx = antico->value("last_highlight_indx", 0).toInt();
	// defaults to our AquaBlue
        lastColor = QColor(antico->value("last_highlight", QColor(0, 96, 202).name()).toString());
	dblClickMinimize = antico->value("minimize_dbl_click", false).toBool();
	antico->endGroup();

	qts = new QSettings(QLatin1String("Trolltech"));
}


bool Appearance::saveSettings()
{
	return true;
}

void Appearance::setupStyles()
{
	QStringList styles = QStyleFactory::keys();
	styles.sort();
	qts->beginGroup(QLatin1String("Qt"));
	QString currentStyle = qts->value(QLatin1String("style")).toString();
	qts->endGroup();
	if (currentStyle.isNull())
		currentStyle = QApplication::style()->objectName();
	// now make current style active, if it is present in styles list,
	// ... or insert it into Styles Combo;
	int indx = styles.indexOf(QRegExp(currentStyle, Qt::CaseInsensitive));
	if (indx < 0) {
		styles << currentStyle;
		indx = styles.count() - 1;
	}

	ui.styleCb->clear();
	ui.styleCb->insertItems(0, styles);
	ui.styleCb->setCurrentIndex(indx);
	lastStyleIndx = indx;
}

void Appearance::setupColors()
{
	// AquaBlue
        QColor c0(0, 96, 202);
	ui.colorCb->insertItem(0, "AquaBlue");
	ui.colorCb->setItemData(0, c0, Qt::DecorationRole);

	// Sea Blue
	QColor c1(0, 0, 255);
	ui.colorCb->insertItem(1, "Sea Blue");
	ui.colorCb->setItemData(1, c1, Qt::DecorationRole);

	// Graphite
	QColor c2(110, 110, 110);
	ui.colorCb->insertItem(2, "Graphite");
	ui.colorCb->setItemData(2, c2, Qt::DecorationRole);

	// Silver
	QColor c3(220, 220, 200);
	ui.colorCb->insertItem(3, "Silver");
	ui.colorCb->setItemData(3, c3, Qt::DecorationRole);

	// Sunny
	QColor c4(255, 255, 0);
	ui.colorCb->insertItem(4, "Sunny");
	ui.colorCb->setItemData(4, c4, Qt::DecorationRole);

	// Orange
	QColor c5(255, 170, 0);
	ui.colorCb->insertItem(5, "Orange");
	ui.colorCb->setItemData(5, c5, Qt::DecorationRole);

	// Emerald Green
	QColor c6(85, 230, 20);
	ui.colorCb->insertItem(6, "Emerald Green");
	ui.colorCb->setItemData(6, c6, Qt::DecorationRole);

	// Girlish
	QColor c7(220, 20, 220);
	ui.colorCb->insertItem(7, "Girlish");
	ui.colorCb->setItemData(7, c7, Qt::DecorationRole);
}

QStringList Appearance::rebuildPalette(const QColor &highlight)
{
	QStringList pal;

	for (int i = 0; i < QPalette::NColorRoles; i++) {
		if ((QPalette::ColorRole) i == QPalette::Highlight) {
			pal << highlight.name();	
		} else
			pal << palette().color(QPalette::Active,
				(QPalette::ColorRole) i).name();
	}
	return pal;
}
