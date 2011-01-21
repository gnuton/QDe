//////////////////////////////////////////
//  File       	: intnlpref.h           //
//  Written by	: ludmiloff@gmail.com   //
//  Copyright  	: GPL2                  //
//////////////////////////////////////////


#ifndef __INTNL_H
#define __INTNL_H

#include <QtGui>
#include "prefwidget.h"
#include "ui_international.h"

class RulesInfoItem : public QTreeWidgetItem
{
public:
	enum RuleItemType {Section=QTreeWidgetItem::UserType, Model, Layout, Option};

        RulesInfoItem(QTreeWidgetItem *parent, int type, const QString &name, const QString &descr, const QString &variant);
        RulesInfoItem(QTreeWidget *parent, const QString name);
};


class VariantInfo
{
public:
	QString variant;
	QString name;

	bool operator ==(const VariantInfo &other) const; //{
		//return (name==other.name && variant==other.variant);
	//}
};

class IntnlPref : public PrefWidget
{
        Q_OBJECT
public:
        IntnlPref(QWidget *parent = 0);

        virtual void readSettings();
        virtual bool saveSettings();


        QString keyboardModel;
	QMultiMap <QString, VariantInfo> activeLayouts;

public slots:
	void onItem(QTreeWidgetItem*, int);
	void onShowHideKbdCtrl(bool);

private:
        Ui::intnlFrm ui;

	RulesInfoItem *modelsRoot, *layoutsRoot;

        const QString findX11Dir();
        const QString findXkbRulesFile(const QString &x11Dir, Display *dpy);
        void loadRules(const QString& file, const QString &x11Dir, bool layoutsOnly);
        QStringList getVariants(const QString& layout, const QString& x11Dir);
};

#endif
