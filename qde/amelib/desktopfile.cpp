#include <DesktopFile>

AmeDesktopFile::AmeDesktopFile(const QString &fileName, QObject *parent)
	: AmeSimpleRC(fileName, '=', parent)
{
}
 
bool AmeDesktopFile::isDesktopFile(const QString &path)
{
	return (path.length() > 8 && path.endsWith(QLatin1String(".desktop")));
}

QString AmeDesktopFile::readName() const
{
	return map.value("Name", QString());
}

QString AmeDesktopFile::readCommand() const 
{
	return map.value("Exec", QString());
}

QStringList AmeDesktopFile::readCategories() const
{
	QString data = map.value("Categories", "Other");
	return data.split(';', QString::SkipEmptyParts);
}

bool AmeDesktopFile::isVisible()
{
	QString data = map.value("NoDisplay",QString());
	return data.toLower()!="true";
}