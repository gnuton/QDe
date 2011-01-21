#ifndef __AMESTATICICONLIST_H
#define __AMESTATICICONLIST_H

#include <Global>
#include <Icon>
#include <QtGui>

class AME_EXPORT AmeIconLink : public AmeIcon
{
public:
	AmeIconLink(QWidget *parent = 0);
	AmeIconLink(const QString &linkName, const QString &linkIconPath, 
		const QString &linkCmd, const QString &linkComment, QWidget *parent = 0);
	~AmeIconLink();

	//QString name() const;
	//QString icon() const;
	QString link() const;
	QString comment() const;

private:
	class IconLinkPrivate;
	IconLinkPrivate * d;
};


class AME_EXPORT AmeStaticIconList : public QWidget
{
	Q_OBJECT
public:

	enum RowType {
		Odd, Even
	};

	AmeStaticIconList(const QString &title, int rowType = Odd, QWidget *parent = 0);
	virtual ~AmeStaticIconList();

	void addIcon(AmeIconLink *icon);
	virtual QSize sizeHint () const;

protected:
	void paintEvent (QPaintEvent *event);


private:
	class StaticIconListPrivate;
	StaticIconListPrivate * d;
};

#endif

