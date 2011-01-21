//////////////////////////////////////////
//  File       	: appearance.h		//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL			//
//////////////////////////////////////////

#ifndef __APPEARANCE_H
#define __APPEARANCE_H

#include <QtGui>
#include "prefwidget.h"
#include "ui_appearance.h"

class Appearance : public PrefWidget
{
        Q_OBJECT
public:
        Appearance(QWidget *parent = 0);
        ~Appearance();

        virtual void readSettings();
        virtual bool saveSettings();

public slots:
        void onChangeColor(int indx);
        void onChangeStyle(int indx);
        void applyChanges();
        void onDblClickOption(bool);

protected:

private:
        Ui::appearanceFrm ui;
        int lastColorIndx, lastStyleIndx;
        QColor lastColor;
        bool dblClickMinimize;

        QSettings *qts;

        void setupStyles();
        void setupColors();
        QStringList rebuildPalette(const QColor &highlight);
};

#endif

