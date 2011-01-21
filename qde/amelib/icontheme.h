//////////////////////////////////////////
//  File      : icontheme.h		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#ifndef __AMEICONTHEME_H
#define __AMEICONTHEME_H

#include <QString>
#include <QIcon>
#include <QPixmap>

#include <Global>
#include <AmeDirs>
#include <Settings>

class AmeIconThemeDir
{
public:
        enum Context {
                Any,
                Action,
                Application,
                Device,
                Place,
                MimeType,
                Animation,
                Category,
                Emblem,
                Emote,
                International,
                StatusIcon,
        };

        explicit AmeIconThemeDir(const QString &rootDir, const QString &dir, AmeSettings &config);
	~AmeIconThemeDir();

	bool isValid() const;
	QString dir() const;
        int size() const;
        int context() const;

	QStringList icons() const;
	QString iconPath(const QString &name) const;
private:
	bool mValid;
	QString mDir;
        int mSize;
        int mCtx;
};


class AME_EXPORT AmeIconTheme : public QObject
{
public:

	enum States {
		DefaultState,
		ActiveState,
		DisabledState,
	};

	explicit AmeIconTheme(const QString &dir, QObject *parent=0);
	virtual ~AmeIconTheme();


	static AmeIconTheme* global();
	
	QString name() const;
        QString comment() const;
	QString dir() const;

	bool setTheme(const QString &theme);

        QPixmap loadIcon(const QString &name, int ctx, int size) const;

private:

protected:
	QString themeName;
	QString themeDir;
        QString themeComment;
        bool themeValid;
	
        QMultiMap <int, AmeIconThemeDir *> mDirs;
        QList <int> mSizes;
	
        void paint(QPainter *paint, const QRect &rect, const QString &name, QIcon::Mode mode, QIcon::State state);
	
};

#endif /* __AMEICONTHEME_H */

