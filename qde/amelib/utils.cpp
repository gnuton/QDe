//////////////////////////////////////////
//  File      : utils.cpp		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL			//
//////////////////////////////////////////

#include <Utils>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

namespace AmeUtils
{
}

QString AmeUtils::homeDir(const QString &user)
{
	if (user.isEmpty())
		return QDir::homePath();
	const passwd *p = getpwnam(user.toAscii());
	if (p) {
		return QString::fromLocal8Bit(p->pw_dir);
	}
	return QString("");
}


#ifdef Q_WS_WIN
#define ESCAPE '^'
#else
#define ESCAPE '\\'
#endif

QString AmeUtils::tildeExpand(const QString &path)
{
	if (path.length() && path[0] == QLatin1Char('~')) {
		int pos = path.indexOf(QLatin1Char('/'));
		if (pos < 0)
			return homeDir(path.mid(1));
		QString ret = homeDir(path.mid(1, pos-1));
		if (!ret.isNull())
			ret += path.mid(pos);
		return ret;
	} else if (path.length() > 1 && path[0] == QLatin1Char(ESCAPE) && path[1] == QLatin1Char('~')) {
		return path.mid(1);
	}
	return path;
}
// "stolen" from kdelibs/kdecore/kernel/kstandarddirs.cpp
// Copyright (C) 1999 Sirtaj Singh Kang <taj@kde.org>
// Copyright (C) 1999,2007 Stephan Kulow <coolo@kde.org>
bool AmeUtils::makeDir(const QString &dir, int mode)
{
	if (QDir::isRelativePath(dir))
		return false;
	QString target = dir;
	uint len = target.length();
	
	if (!dir.endsWith('/'))
		target.append('/');
	
	QString base("");
	uint i = 1;
	
	while (i < len) {
		struct stat64 st;
		int pos = target.indexOf('/', i);
		base += target.mid(i-1, pos - i + 1);
		QByteArray baseEncoded = QFile::encodeName(base);
		if (stat64(baseEncoded, &st) != 0) {
			// Directory does not exists or is a symlynk
			if (lstat64(baseEncoded, &st) == 0)
				(void) unlink(baseEncoded); // try removing
			if (mkdir(baseEncoded, static_cast<mode_t>(mode)) != 0) {
				qDebug() << "error while trying to create" << baseEncoded.constData() << "directory";
				return false;
			}
		}
		i = pos + 1;
	}
	return true;
}

