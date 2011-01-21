#include <StaticIconList>

class AmeIconLink::IconLinkPrivate
{
public:
	QString link;
	QString comment;
};


AmeIconLink::AmeIconLink(QWidget *parent)
	: AmeIcon(parent),
	  d(new IconLinkPrivate)
{
}

AmeIconLink::AmeIconLink(const QString &linkName, const QString &linkIconPath, 
				const QString &linkCmd, const QString &linkComment, QWidget *parent)
	: AmeIcon(linkName, QPixmap(linkIconPath), AmeIcon::average, parent),
	  d(new IconLinkPrivate)
{
	d->link = linkCmd;
	d->comment = linkComment;
}
	  

/*
QString AmeIconLink::name() const
{
	return d->name;
}

QString AmeIconLink::icon() const
{
	return d->icon;
}
*/

QString AmeIconLink::link() const
{
	return d->link;
}

QString AmeIconLink::comment() const
{
	return d->comment;
}
AmeIconLink::~AmeIconLink()
{
	delete d;
	d = NULL;
}


class AmeStaticIconList::StaticIconListPrivate
{
public:
	QHBoxLayout *layout;
	QString title;
	int nItems;
	int recomendHeight, recomendWidth;
	int rowType;
};

AmeStaticIconList::AmeStaticIconList(const QString &title, int rowType, QWidget *parent)
	: QWidget(parent),
	  d (new StaticIconListPrivate)
{
	d->layout = new QHBoxLayout;
	d->title = title;
	d->layout->setContentsMargins(2, 16, 0, 4);
	d->layout->setSpacing(3);
	d->layout->addStretch();
	setLayout(d->layout);
	d->nItems = 0;
	d->recomendHeight = 0;
	d->recomendWidth = 0;
	d->rowType = rowType;

	QFont f = QFont(font());
	f.setBold(true);
	//f.setWeight(75);
	f.setPointSize(9);
	setFont(f);
}

AmeStaticIconList::~AmeStaticIconList()
{
	delete d;
	d = NULL;
}

QSize AmeStaticIconList::sizeHint() const
{
	if (d->nItems > 0) {
		return (QSize(d->recomendWidth, d->recomendHeight + 32));
	} else 
		return QSize(0, 0);
}

void AmeStaticIconList::addIcon(AmeIconLink *icon)
{
	d->layout->insertWidget(d->nItems, icon);
	d->nItems += 1;
	if (d->recomendHeight < icon->height())
		d->recomendHeight = icon->height();
	d->recomendWidth += icon->width();
}


void AmeStaticIconList::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect rect = painter.window();
	QColor base, bcolor;
	if (d->rowType == Odd) {
		base = QColor(palette().window().color().darker(102));
	} else {
		base = QColor(palette().window().color().darker(106));
		bcolor = QColor(palette().window().color().darker(115));
	}
	painter.fillRect(rect, base);
	QRect offset(rect.left()+10, rect.top() + 4, rect.width(), rect.height());
	painter.drawText(offset, Qt::AlignRight || Qt::AlignTop, d->title);
	if (d->rowType == Even) {
		painter.setPen(QPen(QBrush(bcolor), 2));
		painter.drawLine(0, 0, rect.width(), 0);
		painter.drawLine(0, rect.height(), rect.width(), rect.height());
	}
}

