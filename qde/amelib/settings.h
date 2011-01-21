//////////////////////////////////////////
//  File      : settings.h		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#ifndef __AMESETTINGS_H
#define __AMESETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QSettings>
#include <Global>

class AME_EXPORT AmeSettings : public QSettings
{

/**
 * @short: Extended QSettings class
 *
 * This class provide some extra methods for reading/writing
 * list of strings, separated by given separator in
 * very compact way
 *
 * Example:
 * key=value1,value2,value3....,valueN
 *
 * Useful when reading themes descriptor file
 *
 */

public:
	AmeSettings(const QString &fileName, Format format = QSettings::IniFormat, QObject *parent=0);

	// write
	void setListValue(const QString &key, const QStringList &val, char sep = ',');

	// read
	QStringList listValue(const QString &key, char sep = ',') const;
	QStringList listValue(const QByteArray &key, char sep = ',') const;
	QStringList listValue(const char *key, char sep = ',') const;
};

#endif /* __AMESETTINGS_H */
