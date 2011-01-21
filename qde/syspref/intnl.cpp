//////////////////////////////////////////////////////////////////////////////////
//  File       	: intnlpref.cpp                                                 //
//  Written by	: ludmiloff@gmail.com                                           //
//  Copyright  	: GPL2                                                          //
//  Note        : Large portions of this file were                              //
//                taken from AnticoXkb project                                  //
//                http://qt-apps.org/content/show.php/Antico+XKB?content=101667 //
//////////////////////////////////////////////////////////////////////////////////

#include "intnl.h"
#include <QX11Info>
#include <QtDBus>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#define display() QX11Info::display()


// Compiler will size array automatically.
static const char* X11DirList[] = {
        "/etc/X11/",
        "/usr/share/X11/",
        "/usr/local/share/X11/",
        "/usr/X11R6/lib/X11/",
        "/usr/X11R6/lib64/X11/",
        "/usr/local/X11R6/lib/X11/",
        "/usr/local/X11R6/lib64/X11/",
        "/usr/lib/X11/",
        "/usr/lib64/X11/",
        "/usr/local/lib/X11/",
        "/usr/local/lib64/X11/",
        "/usr/pkg/share/X11/",
        "/usr/pkg/xorg/lib/X11/"
};

// Compiler will size array automatically.
static const char* rulesFileList[] =
{
        "xkb/rules/base",
        "xkb/rules/xorg",
        "xkb/rules/xfree86"
};

// Macro will return number of elements in any static array as long as the
// array has at least one element.
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static const int X11_DIR_COUNT = ARRAY_SIZE(X11DirList);
static const int X11_RULES_COUNT = ARRAY_SIZE(rulesFileList);

RulesInfoItem::RulesInfoItem(QTreeWidgetItem *parent, int type,
        const QString &name, const QString &descr, const QString &variant)
	: QTreeWidgetItem(parent, type)
{
        setText(0, name);
        setText(1, descr);
        setText(2, variant);
}

RulesInfoItem::RulesInfoItem(QTreeWidget *parent, const QString name)
	: QTreeWidgetItem(parent, Section)
{
        setText(0, name);
}

bool VariantInfo::operator ==(const VariantInfo &other) const
{
	return (name==other.name && variant==other.variant);
}

IntnlPref::IntnlPref(QWidget *parent)
        : PrefWidget(parent)
{
        ui.setupUi(this);
        QStringList hdr;
        hdr << "Name" << "Description" << "Variant";
        ui.layoutsList->setColumnCount(3);
        ui.layoutsList->setHeaderLabels(hdr);

        ui.layoutsList->setColumnWidth(0, 330);
        ui.layoutsList->setColumnWidth(1, 90);
        ui.layoutsList->setColumnWidth(2, 100);

	//connect(ui.layoutsList, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
	//	SLOT(onItem(QTreeWidgetItem*, int)));

	//connect(ui.layoutsList, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this,
	//	SLOT(onItem(QTreeWidgetItem*, int)));

	connect(ui.showInputMnuChk, SIGNAL(clicked(bool)), this, SLOT(onShowHideKbdCtrl(bool)));

        readSettings();
        QString x11Dir = findX11Dir();
        QString rulesFile = findXkbRulesFile(x11Dir, display());
        loadRules(rulesFile, x11Dir, false);

	if (!activeLayouts.empty()) {
		ui.layoutsList->expandItem(layoutsRoot);
	}
        moduleName = "Language settings";
}


void IntnlPref::onItem(QTreeWidgetItem *it, int col)
{
	if (col == 0) {
		QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe",
			"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
			QDBusConnection::sessionBus(), this);

		//qDebug() << "item click";
		if (it->type() == RulesInfoItem::Model) {
			if (it->checkState(0) == Qt::Checked) {
				keyboardModel = it->text(1);
				qDebug() << keyboardModel;
				
				QTreeWidgetItem *m;
				for (int indx = 0; indx < modelsRoot->childCount(); indx++) {
					m = modelsRoot->child(indx);
					if (it == m) continue;
					m->setCheckState(0, Qt::Unchecked);
				}
			}

			saveSettings();
			// Notify AnticoDeluxe WM for changing keyboard model
			if (!iface->isValid())
				qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
			else {
				iface->call("changeKbdModel", keyboardModel);
			}

		} else if (it->type() == RulesInfoItem::Layout || it->type() == RulesInfoItem::Option) {
			//QString v = it->text(2);
			QString l = it->text(1);
			//QString name = it->text(0);
			VariantInfo vi;
			vi.variant = it->text(2);
			vi.name = it->text(0);

			if (it->checkState(0) == Qt::Checked) {
				activeLayouts.insert(l, vi);
			} else {
				activeLayouts.remove(l, vi);
			}
			saveSettings();
			// Notify AnticoDeluxe WM for changing keyboard layouts
			if (!iface->isValid())
				qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
			else {
				iface->call("changeKbdLayouts");
			}
		}
	}
}

void IntnlPref::onShowHideKbdCtrl(bool show)
{
	QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe",
			"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
			QDBusConnection::sessionBus(), this);

	//bool show = ui.showInputMnuChk->isChecked();
	if (!iface->isValid())
		qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
	else {
		iface->call("showInputMenu", show);
	}
	saveSettings();

}

void IntnlPref::readSettings()
{
        antico->beginGroup("Keyboard");
        keyboardModel = antico->value("model", "pc104").toString();
        activeLayouts.clear();
        int size = antico->beginReadArray("layouts");
        qDebug() << "read " << size << "layouts";
        for (int i = 0; i < size; ++i) {
		antico->setArrayIndex(i);
		VariantInfo vi;
		vi.variant = antico->value("variant", "").toString();
		vi.name = antico->value("name", "").toString();
		activeLayouts.insert(antico->value("layout", "").toString(), vi);
        }
        antico->endArray();
	ui.showInputMnuChk->setChecked(antico->value("show_input_mnu", false).toBool());
        antico->endGroup(); //Keyboard

	if (activeLayouts.isEmpty()) {
		qDebug() << "adding USA fallback layout";
		VariantInfo vi;
		vi.variant = "";
		vi.name = "USA";

		activeLayouts.insert("us", vi);
		saveSettings();
	}
}

bool IntnlPref::saveSettings()
{
	antico->beginGroup("Keyboard");
	antico->setValue("model", keyboardModel);

	int oldSize = antico->beginReadArray("layouts");
	antico->endArray();
	// remove all old layouts
	antico->beginWriteArray("layouts");
	for (int i=0; i <= oldSize; ++i) {
		antico->setArrayIndex(i);
		antico->remove("layout");
		antico->remove("variant");
		antico->remove("name");
		antico->remove("order");
	}
	antico->endArray();

	antico->beginWriteArray("layouts");
	QList<QString> k = activeLayouts.uniqueKeys();
	int i = 0;
	foreach (QString key, k) {
		qDebug() << "saving key " << key;
		int order = 1;
		foreach (VariantInfo vi, activeLayouts.values(key)) {
			antico->setArrayIndex(i);
			qDebug() << "saving key (" << key << ") variant " << vi.variant;
			antico->setValue("layout", key);
			antico->setValue("variant", vi.variant);
			antico->setValue("name", vi.name);
			antico->setValue("order", order);
			order ++;
			i ++;
		}
	}

	antico->endArray();

	antico->setValue("show_input_mnu", ui.showInputMnuChk->isChecked());
	antico->endGroup();
	antico->sync();
        return true;
}

const QString IntnlPref::findX11Dir()
{
        for(int ii=0; ii<X11_DIR_COUNT; ii++) {
                const char* xDir = X11DirList[ii];
                if( xDir != NULL && QDir(QString(xDir) + "xkb/rules").exists() ) {
                        return QString(xDir);
            }
        }
        return NULL;
}

const QString IntnlPref::findXkbRulesFile(const QString &x11Dir, Display *dpy)
{
        QString rulesFile;
        XkbRF_VarDefsRec vd;
        char *tmp = NULL;

        if (XkbRF_GetNamesProp(dpy, &tmp, &vd) && tmp != NULL ) {
                rulesFile = x11Dir + QString("xkb/rules/%1").arg(tmp);
        } else {
                // old way
                for(int ii=0; ii<X11_RULES_COUNT; ii++) {
                        const char* ruleFile = rulesFileList[ii];
                        QString xruleFilePath = x11Dir + ruleFile;
                        if  (QFile(xruleFilePath).exists() ) {
                                rulesFile = xruleFilePath;
                                break;
                        }
                }
        }

        return rulesFile;
}

void IntnlPref::loadRules(const QString& file, const QString &x11Dir, bool layoutsOnly)
{
        XkbRF_RulesPtr xkbRules = XkbRF_Load(QFile::encodeName(file).data(), (char*)"", true, true);
        if (xkbRules == NULL) {
                // throw Exception
                return;
        }

	disconnect(ui.layoutsList, SIGNAL(itemChanged(QTreeWidgetItem*, int)), 0,0);

	modelsRoot = new RulesInfoItem(ui.layoutsList, "Keyboard models");

        for (int i = 0; i < xkbRules->models.num_desc; ++i) {
		RulesInfoItem *m = new RulesInfoItem(modelsRoot, RulesInfoItem::Model,
                        xkbRules->models.desc[i].desc,
                        xkbRules->models.desc[i].name, "");
                if (QString(xkbRules->models.desc[i].name) == keyboardModel)
                        m->setCheckState(0, Qt::Checked);
                else
                        m->setCheckState(0, Qt::Unchecked);
        }

        // try to translate layout names by countries
        // this is poor man's translation as it's good only for layout names and only those which match country names

        QMap <QString, QString> layouts;

        for (int i = 0; i < xkbRules->layouts.num_desc; ++i) {
		layouts.insert(xkbRules->layouts.desc[i].desc, xkbRules->layouts.desc[i].name);
                //qDebug() << xkbRules->layouts.desc[i].desc;
        }

	layoutsRoot = new RulesInfoItem(ui.layoutsList, "Keyboard layouts");
        foreach (QString layoutName, layouts.keys()) {
		QStringList variants = getVariants(layouts.value(layoutName), x11Dir);
                RulesInfoItem *it1;

		// layout description - 2nd column
		QString d = layouts.value(layoutName);

                if (variants.isEmpty()) {
			it1 = new RulesInfoItem(layoutsRoot, RulesInfoItem::Layout,
				layoutName, layouts.value(layoutName), "");
			if (activeLayouts.contains(d))
				it1->setCheckState(0, Qt::Checked);
			else
				it1->setCheckState(0, Qt::Unchecked);

                } else {
                        foreach (QString variant, variants) {
				if (variant == "basic") {
					it1 = new RulesInfoItem(layoutsRoot, RulesInfoItem::Layout,
						layoutName, d, "");
					if (activeLayouts.contains(d))
						it1->setCheckState(0, Qt::Checked);
					else
						it1->setCheckState(0, Qt::Unchecked);

				} else {
					it1 = new RulesInfoItem(layoutsRoot, RulesInfoItem::Option,
						layoutName, d, variant);
					if (activeLayouts.contains(d)) {
						QList <VariantInfo> vlist = activeLayouts.values(d);
						foreach (VariantInfo vi, vlist) {
							if (vi.variant == variant) {
								it1->setCheckState(0, Qt::Checked);
								break;
							}
							else
								it1->setCheckState(0, Qt::Unchecked);
						}
					} else 
						it1->setCheckState(0, Qt::Unchecked);
				}
                        }
                }
        }


	connect(ui.layoutsList, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this,
		SLOT(onItem(QTreeWidgetItem*, int)));

        if( layoutsOnly == true ) {
                XkbRF_Free(xkbRules, true);
                return;
        }


/*
        for (int i = 0; i < xkbRules->options.num_desc; ++i) {
                QString optionName = xkbRules->options.desc[i].name;
                //qDebug() << " option: " << optionName;
                int colonPos = optionName.indexOf(':');
                QString groupName = optionName.mid(0, colonPos);


                if( colonPos != -1 ) {
                        //qDebug() << " option: " << optionName;

                        if( ! rulesInfo->optionGroups.contains( groupName ) ) {
                                rulesInfo->optionGroups.insert(groupName, createMissingGroup(groupName));
                                //  qDebug() << " added missing option group: " << groupName;
                        }

                        XkbOption option;
                        option.name = optionName;
                        option.description = xkbRules->options.desc[i].desc;
                        //option.group = &rulesInfo->optionGroups[ groupName ];
                        rulesInfo->optionGroups[ groupName ].options <<option;
                        rulesInfo->options.insert(optionName, option);
                } else {
                        if( groupName == "Compose" )
                                groupName = "compose";
                        if( groupName == "compat" )
                                groupName = "numpad";

                        XkbOptionGroup optionGroup;
                        optionGroup.name = groupName;
                        optionGroup.description = xkbRules->options.desc[i].desc;
                        optionGroup.exclusive = isGroupExclusive( groupName );

                        //qDebug() << " option group: " << groupName;
                        rulesInfo->optionGroups.insert(groupName, optionGroup);
                }
        }
*/
        XkbRF_Free(xkbRules, true);
}

/* pretty simple algorithm - reads the layout file and
    tries to find "xkb_symbols"
    also checks whether previous line contains "hidden" to skip it
*/

QStringList IntnlPref::getVariants(const QString& layout, const QString& x11Dir)
{

        QStringList result;
        QString file = x11Dir + "xkb/symbols/";

        // workaround for XFree 4.3 new directory for one-group layouts
        if( QDir(file+"pc").exists() )
                file += "pc/";

        file += layout;
        //  qDebug() << "reading variants from " << file;

        QFile f(file);
        if (f.open(QIODevice::ReadOnly)) {
                QTextStream ts(&f);
                QString line;
                QString prev_line;

                while ( ts.status() == QTextStream::Ok ) {
                        prev_line = line;

                        QString str = ts.readLine();

                        if( str.isNull() )
                                break;

                        line = str.simplified();

                        if (line[0] == '#' || line.left(2) == "//" || line.isEmpty())
                                continue;

                        int pos = line.indexOf("xkb_symbols");
                        if (pos < 0)
                                continue;

                        if( prev_line.indexOf("hidden") >=0 )
                                continue;

                        pos = line.indexOf('"', pos) + 1;
                        int pos2 = line.indexOf('"', pos);
                        if( pos < 0 || pos2 < 0 )
                                continue;

                        result.append(line.mid(pos, pos2-pos));
			//qDebug() << "adding variant " << line.mid(pos, pos2-pos);
                }
                f.close();
        }
        return result;
}

