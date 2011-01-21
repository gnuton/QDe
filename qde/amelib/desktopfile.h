#ifndef __AMEDESKTOP_FILE_H
#define __AMEDESKTOP_FILE_H

#include <Global>
#include <QString>
#include <QStringList>

#include <Icon>
#include <SimpleRC>

class AME_EXPORT AmeDesktopFile : public AmeSimpleRC
{
public:
	AmeDesktopFile(const QString &fileName, QObject *parent=0);
	
	static bool isDesktopFile(const QString &path);
	QString readName() const;
	QStringList readCategories() const;
	QString readCommand() const;
	bool isVisible();
        AmeIcon icon() const;
};

#endif

