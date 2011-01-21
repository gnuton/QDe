//////////////////////////////////////////
//  File      : icon.cpp		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#include <Icon>
#include <QPainter>

class AmeIcon::IconPrivate
{
public:
	QString label;
	QString fullText;
	QPixmap pix;
	QPixmap gray_pix;
	int scaleFactor;
	bool drawBorder;
	bool elideMode;
	int state;
        AmeIconTheme *theme;
};

AmeIcon::AmeIcon(QWidget *parent)
        : QWidget(parent),
          d (new IconPrivate)
{
        d->label = "";
        d->fullText = "";
        d->scaleFactor = normal;
        d->drawBorder = false;
        d->elideMode = true;
        d->state = Normal;
}

AmeIcon::AmeIcon(const QString &name, const QString &label, int scale, 
			AmeIconTheme *theme, QWidget *parent) 
	: QWidget(parent),
	  d (new IconPrivate)
{
        d->scaleFactor = scale;
        d->drawBorder = false;
        d->elideMode = true;
        d->state = Normal;
        adjustFont();
        setLabel(label);
        d->theme = theme;

        int s = scale - 4;
        d->pix = theme->loadIcon(name, AmeIconThemeDir::Any, s);
        d->gray_pix = QIcon(d->pix).pixmap(QSize(s, s), QIcon::Disabled);
}

AmeIcon::AmeIcon(const QString &label, const QPixmap &copy, int scale, QWidget *parent)
	: QWidget(parent),
	  d (new IconPrivate)
{
	d->scaleFactor = scale;
	d->drawBorder = false;
	d->elideMode = true;
	d->state = Normal;
	adjustFont();
	setLabel(label);

	int s = scale - 4;
	d->pix = QPixmap(copy.scaled(QSize(s, s), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	d->gray_pix = QIcon(d->pix).pixmap(QSize(s, s), QIcon::Disabled);
}


AmeIcon::~AmeIcon()
{
	delete d;
	d = NULL;
}


void AmeIcon::setLabel(const QString &label)
{
	if (d->elideMode) {
		d->fullText = label;
		squeezeText();
	} else {
		d->label = label;
		d->fullText = label;
	}
	update();
}


void AmeIcon::adjustFont()
{
	QFont f = QFont(font());
	if (d->scaleFactor == AmeIcon::large) {
		f.setPointSize(12);
		f.setBold(true);
	} else 	if (d->scaleFactor == AmeIcon::normal) {
		f.setPointSize(10);		
		f.setBold(true);
	} else if (d->scaleFactor == AmeIcon::medium) {
		f.setPointSize(9);		
		f.setBold(true);		
	} else if (d->scaleFactor == AmeIcon::average) {
		f.setPointSize(8);
		f.setBold(false);
	}
	else if (d->scaleFactor == AmeIcon::small) {
		f.setPointSize(7);
		f.setBold(false);
	}
	//f.setWeight(75);
	setFont(f);
}

QSize AmeIcon::sizeHint() const
{
	return (QSize(d->scaleFactor * 2, d->scaleFactor + 5 + fontMetrics().height() * 2));
}

int AmeIcon::width()
{
	return d->scaleFactor * 2;
}

int AmeIcon::height()
{
	return d->scaleFactor + 5 + fontMetrics().height() * 2;
}

void AmeIcon::squeezeText()
{
	QFontMetrics fm(fontMetrics());
	int charWidth = fm.width('D');
	int labelWidth = width();
	int lineWidth = labelWidth / charWidth;

	QString first, second;

	QStringList lines = d->fullText.split("\n");
	if (lines.size() >= 2) {
		first = lines.at(0);
		lines.removeFirst();
		second = lines.join(" ");
	} else {	
		first = d->fullText.left(lineWidth); // left(20)
		int remaining = d->fullText.size() - lineWidth; // 30 - 20 = 10
		if (remaining > 0)
			second = d->fullText.right(remaining); // right(10)
	}

	if (!second.isEmpty()) {
		if (fm.width(second) >= lineWidth) {
			d->label = first + "\n" + fm.elidedText(second, Qt::ElideMiddle, labelWidth);
			setToolTip(d->fullText);
		} else {
			d->label = first + "\n" + second;
		}
	} else {
		d->label = first + "\n";
		setToolTip(QString());
	}
}

void AmeIcon::drawBorder(bool draw)
{
	d->drawBorder = draw;
	update();
}

void AmeIcon::scale(int factor)
{
}

void AmeIcon::paintIcon()
{
	if (!d->pix.isNull()) {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		QRect rect = painter.window();
		
		int left = (rect.width() - d->pix.width()) / 2;
		
		if (d->drawBorder) {
			QColor base;
			base = QColor(palette().window().color());
			painter.fillRect(5, 4, rect.width()-7, rect.height()-6, QBrush(base.darker(180)));
			painter.setPen(QPen(QBrush(QColor(254, 254, 254)), 3));
			painter.drawRoundedRect(4, 3, rect.width()-6, rect.height()-5, 4, 4);
		}

		QRect pixRect(left, 7, d->pix.width(), d->pix.height());
		if (d->state == Normal)
			painter.drawPixmap(pixRect, d->pix);
		else
			painter.drawPixmap(pixRect, d->gray_pix);	
	}
}

void AmeIcon::paintLabel()
{
	if (!d->label.isEmpty()) {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		QRect rect = painter.window();
		QStringList lines = d->label.split("\n");
		
		int y = d->scaleFactor + 4;
		QRect offset(rect.left(), y, rect.width(), rect.height() - y);
		if (d->drawBorder)
			painter.setPen(QColor(255, 255, 255));
		painter.drawText(offset, Qt::AlignHCenter | Qt::AlignTop, lines.at(0));

		if (lines.size() > 1) {
			y += fontMetrics().height();
			QRect offset(rect.left(), y, rect.width(), rect.height() - y);
			painter.drawText(offset, Qt::AlignHCenter | Qt::AlignTop, lines.at(1));
		}
	}
}

void AmeIcon::paintEvent(QPaintEvent *event)
{
	paintIcon();
	paintLabel();
}


void AmeIcon::mousePressEvent(QMouseEvent *event)
{
        QWidget::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {
                d->state = Selected;
                update();
        }
}

void AmeIcon::mouseReleaseEvent(QMouseEvent *event)
{
        QWidget::mouseReleaseEvent(event);
        if (event->button() == Qt::LeftButton) {
                d->state = Normal;
                update();
                emit clicked();
        }
}
