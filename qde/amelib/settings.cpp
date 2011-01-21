//////////////////////////////////////////
//  File      : settings.h		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#include <Settings>

AmeSettings::AmeSettings(const QString &fileName, Format format, QObject *parent)
  : QSettings(fileName, format, parent)
{
}

void AmeSettings::setListValue(const QString &key, const QStringList &val, char sep)
{
    setValue(key, val.join(QString(sep)));
}

QStringList AmeSettings::listValue(const QString &key, char sep) const
{
	const QString data = value(key, QVariant("")).toString();
	return data.split(sep, QString::SkipEmptyParts);
}

QStringList AmeSettings::listValue(const QByteArray &key, char sep) const
{
	return listValue(QString(key), sep);
}

QStringList AmeSettings::listValue(const char *key, char sep) const
{
	return listValue(QString(key), sep);
}
