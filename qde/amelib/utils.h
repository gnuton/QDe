#ifndef __AMEUTILS_H
#define __AMEUTILS_H

#include <QtCore>
#include <Global>

namespace AmeUtils {

QString AME_EXPORT homeDir(const QString &user);

QString AME_EXPORT tildeExpand(const QString &path);

bool AME_EXPORT makeDir(const QString &dir, int mode);

}

#endif /* __AMEUTILS_H */
