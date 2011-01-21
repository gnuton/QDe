//////////////////////////////////////////
//  File      : amedirs.h		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#ifndef __AMEDIRS_H
#define __AMEDIRS_H

#include <Global>
#include <QDir>
#include <QString>

/**
 * @short System or application wide dirs
 *
 * This class is a central way for getting system or application wide 
 * default dirs
 * in form: 
 *
 * name=default_value
 * 
 */

class AME_EXPORT AmeDirs
{
public:

	enum DefaultsScope {
		SystemWide,
		Application,
	};

	enum DefaultDirs {
		Local,
		Home,
		System,
		Desktop,
		Autostart,
		Documents,
		Music,
		Videos,
		Configs,
		IconTheme,
		SoundTheme,
		ThisApplication,
	};

	AmeDirs();
	virtual ~AmeDirs();
	
	static AmeDirs* global();	
	QString stdDir(int type);

private:
	void addStdDirs();
	bool createStdDirs();
};

#endif

