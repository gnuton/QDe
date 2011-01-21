//////////////////////////////////////////////////////////
//  File       : adxsettings.cpp          		//
//  Written by : ludmiloff@gmail.com 			//
//  Some copy/paste code from original Antico project	//
//  Copyright  : GPL2					//
//////////////////////////////////////////////////////////

#include <AmeDirs>
#include "adx.h"

void Adx::readAdxSettings(void)
{
	// default path = $HOME/.ame/config/AnticoDeluxe"
	stg = new AmeSettings(AmeDirs::global()->stdDir(AmeDirs::Configs) + "/AnticoDeluxe", QSettings::IniFormat);

        /////////////////////////////////////////////////////////////////////////
	stg->beginGroup("Dockicon");
	stg->setValue("d_icon_pix", QCoreApplication::applicationDirPath() + "/../share/themes/antico/default/dockicon.png");
	stg->setValue("title_color", "#ffffff"); //white
	stg->endGroup(); //Dockicon
	/////////////////////////////////////////////////////////////////////////
	stg->beginGroup("Deskicon");
	stg->setValue("d_icon_pix", QCoreApplication::applicationDirPath() + "/../share/themes/antico/default/deskicon.png");
	stg->setValue("name_color", "#ffffff"); //white
	stg->endGroup(); //Deskicon
	/////////////////////////////////////////////////////////////////////////
	stg->beginGroup("Appearance");
	// Highlight color defaults to our AquaBlue
	QColor lastColor = QColor(stg->value("last_highlight", QColor(0, 0, 255).name()).toString());
        stg->setValue("last_highlight", lastColor.name());
	setHighlightColor(lastColor);
	minimizeDblClick = stg->value("minimize_dbl_click", false).toBool();
        stg->setValue("minimize_dbl_click", minimizeDblClick);
	stg->endGroup();
	/////////////////////////////////////////////////////////////////////////
	stg->sync();
	
	if (stg->status() == QSettings::AccessError)
		qDebug () << "Error on setting AnticoDeluxe.conf";
}
