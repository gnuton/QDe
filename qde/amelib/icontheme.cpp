#include <QFile>
#include <QDir>

#include <IconTheme>
#include <AmeDirs>
#include <sys/stat.h>


AmeIconThemeDir::AmeIconThemeDir(const QString &rootDir, const QString &dir, AmeSettings &config)
{
        QString path = rootDir + "/" + dir;
        if (QDir(path).exists()) {
                config.beginGroup(dir);

                mDir = path;
                mSize = config.value("size", 0).toInt();

                QString c = config.value("Context", "any").toString();
                c = c.toLower();
                if (c == "any")
                        mCtx = AmeIconThemeDir::Any;
                else if (c == "actions")
                        mCtx = AmeIconThemeDir::Action;
                else if (c == "aplications")
                        mCtx = AmeIconThemeDir::Application;
                else if (c == "devices")
                        mCtx = AmeIconThemeDir::Device;
                else if (c == "places")
                        mCtx = AmeIconThemeDir::Place;
                else if (c == "mimetypes")
                        mCtx = AmeIconThemeDir::MimeType;
                else if (c == "emblems")
                        mCtx = AmeIconThemeDir::Emblem;
                else if (c == "animations")
                        mCtx = AmeIconThemeDir::Animation;
                else if (c == "categories")
                        mCtx = AmeIconThemeDir::Category;
                else if (c == "emotes")
                        mCtx = AmeIconThemeDir::Animation;

                else
                        mCtx = AmeIconThemeDir::Any;

                config.endGroup();

                if (mSize == 0)
                        mValid = false;
                else
                        mValid = true;
        } else
                mValid = false;
}

AmeIconThemeDir::~AmeIconThemeDir()
{
}

bool AmeIconThemeDir::isValid() const
{
	return mValid;
}

QString AmeIconThemeDir::dir() const
{
	return mDir;
}

int AmeIconThemeDir::size() const
{
        return mSize;
}

int AmeIconThemeDir::context() const
{
        return mCtx;
}

QStringList AmeIconThemeDir::icons() const
{
        // TODO: FINISH THIS
	return QStringList();
}

QString AmeIconThemeDir::iconPath(const QString &name) const 
{
        if (!mValid)
                return QString();

        QString file = mDir + "/" + name + ".png";

        if (access(QFile::encodeName(file), R_OK) == 0)
                return file;

	return QString();
}


AmeIconTheme::AmeIconTheme(const QString &dir, QObject *parent)
        : QObject(parent),
        themeDir(dir),
        themeValid(false)
{
        setTheme(themeDir + "/default");
}

AmeIconTheme::~AmeIconTheme()
{
}

QString AmeIconTheme::name() const
{
	return themeName;
}

QString AmeIconTheme::comment() const
{
        return themeComment;
}

QString AmeIconTheme::dir() const
{
	return themeDir;
}

bool AmeIconTheme::setTheme(const QString &theme)
{
        mDirs.clear();
        mSizes.clear();

	// 1. check if file index.theme or index.desktop exists
	QString fullPath = themeDir + theme + "/";
	QString fileName, themeFile(fullPath + "index.theme"), desktopFile(fullPath + "index.desktop");
	if (QFile::exists(themeFile)) 
		fileName = themeFile;
	else if (QFile::exists(desktopFile))
		fileName = desktopFile;
        else {
                themeValid = false;
		return false;
        }
	
	themeName = theme;
	// 2. parse theme config file
	AmeSettings config(fileName);

        config.beginGroup("Icon Theme");
        const QStringList dirs = config.listValue("Directories");
        themeComment = config.value("Comment", "").toString();
        config.endGroup();

        QStringList::ConstIterator it;
	for (it = dirs.begin(); it != dirs.end(); ++it) {
                AmeIconThemeDir *d = new AmeIconThemeDir(fullPath, *it, config);
                if (d->isValid()) {
                        mDirs.insert(d->size(), d);
                }
	}
        mSizes = mDirs.uniqueKeys();
        if (mSizes.count() > 0 && mDirs.count() > 0)
                themeValid = true;
        else
                themeValid = false;
        return themeValid;
}

QPixmap AmeIconTheme::loadIcon(const QString &name, int ctx, int size) const
{
        if (!themeValid)
                return NULL;

        QString path;
        QList <AmeIconThemeDir *> dirs;
        int prefSize;

        if (mDirs.contains(size)) {
                // exact size match found
                dirs = mDirs.values(size);
        } else {
                prefSize = mSizes.last();
                QList<int>::ConstIterator it = mSizes.constEnd();
                // search the best match, preferably greater size dir
                while (it != mSizes.constBegin()) {
                        if (*it < prefSize)
                                prefSize = *it;
                        if (*it > prefSize)
                                break;
                        --it;
                }
                dirs = mDirs.values(prefSize);
        }
        // ok. now have a list of AmeIconThemeDir's with the best icon size, lets find the desired icon

        for (int i=0; i <= dirs.count(); i++) {
                if (ctx != AmeIconThemeDir::Any && ctx != dirs.at(i)->context())
                        continue;
                path = dirs.at(i)->iconPath(name);
                if (path.isEmpty())
                        continue;
                return QPixmap(path);
        }

        return QPixmap();
}

void AmeIconTheme::paint(QPainter *paint, const QRect &rect, const QString &name, QIcon::Mode mode, QIcon::State state)
{
}

AME_GLOBAL_STATIC_WITH_ARGS(AmeIconTheme, globalIconTheme, 
	(AmeDirs::global()->stdDir(AmeDirs::IconTheme)))




