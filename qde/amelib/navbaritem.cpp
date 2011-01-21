/* 
 *  This file is taken from Quartica project (MacNavBar).
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * 
 *	Some modifications made by me. <ludmiloff@gmail.com>
 *	- changed class name to fit Amelib namespace;
 *
 */

// Qt4 Headers
#include <QHBoxLayout>
#include <QLabel>

#include <NavBarItem>

// =============================================================================
//  NavBarItem: PRIVATE Class
// =============================================================================
class AmeNavBarItem::Private {
	public:
		QHBoxLayout *layout;
		QLabel *labelIcon;
		QLabel *labelText;
		QString link;

	public:
		void initialize (AmeNavBarItem *item);
};

void AmeNavBarItem::Private::initialize (AmeNavBarItem *item) {
	// Initialize Members
	layout = new QHBoxLayout;
	labelIcon = new QLabel;
	labelText = new QLabel;
	link = "";

	// Setup Text Label
	labelText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	labelIcon->setFixedWidth(18);

	// Setup Font
	QFont font = labelText->font();
	font.setPixelSize(10);
	labelText->setFont(font);

	// Add Items to Layout
	layout->addWidget(labelIcon);
	layout->addWidget(labelText);

	// Setup Layout
	layout->setContentsMargins(1, 1, 1, 1);
	item->setLayout(layout);
}

// =============================================================================
//  NavBarItem: PUBLIC Constructors/Destructors
// =============================================================================
AmeNavBarItem::AmeNavBarItem (QWidget *parent)
	: AmeSelectableWidget(parent), d(new AmeNavBarItem::Private)
{
	d->initialize(this);
}

AmeNavBarItem::AmeNavBarItem (const QString &text, const QString &link, QWidget *parent)
	: AmeSelectableWidget(parent), d(new AmeNavBarItem::Private)
{
	d->initialize(this);
	d->labelText->setText(text);
	d->link = link;
}

AmeNavBarItem::AmeNavBarItem (const QPixmap& icon, const QString& text, const QString &link, QWidget *parent)
	: AmeSelectableWidget(parent), d(new AmeNavBarItem::Private)
{
	d->initialize(this);

	setIcon(icon);
	d->labelText->setText(text);
	d->link = link;
}

AmeNavBarItem::~AmeNavBarItem() {
	delete d;
	d = NULL;
}

// =============================================================================
//  NavBarItem: PUBLIC Methods
// =============================================================================
void AmeNavBarItem::addSpacing (int size) {
	d->layout->addSpacing(size);
}

void AmeNavBarItem::addWidget (QWidget *widget, int stretch) {
	d->layout->addWidget(widget, stretch);
}


void AmeNavBarItem::insertSpacing (int index, int size) {
	d->layout->insertSpacing(index, size);
}

void AmeNavBarItem::insertWidget (int index, QWidget *widget, int stretch) {
	d->layout->insertWidget(index, widget, stretch);
}

// =============================================================================
//  NavBarItem: PUBLIC Get Properties
// =============================================================================
QString AmeNavBarItem::text (void) const {
	return(d->labelText->text());
}

QString AmeNavBarItem::link(void) const {
	return d->link;
}

const QPixmap *AmeNavBarItem::icon (void) const {
	return(d->labelIcon->pixmap());
}

// =============================================================================
//  NavBarItem: PUBLIC Set Properties
// =============================================================================
void AmeNavBarItem::setLink(const QString &link)
{
	d->link = link;
}

void AmeNavBarItem::setIcon (const QPixmap& icon) {
	if (icon.height() > 20)
		d->labelIcon->setPixmap(icon.scaledToHeight(20, Qt::SmoothTransformation));
	else
		d->labelIcon->setPixmap(icon);
}

void AmeNavBarItem::setText (const QString& text) {
	d->labelText->setText(text);
}

void AmeNavBarItem::setTextColor (const QColor& color) {
	QPalette palette = d->labelText->palette();
	palette.setColor(QPalette::WindowText, color);
	d->labelText->setPalette(palette);
}

// =============================================================================
//  NavBarItem: PUBLIC Slots
// =============================================================================

// =============================================================================
//  NavBarItem: PROTECTED Methods
// =============================================================================

// =============================================================================
//  NavBarItem: PRIVATE Slots
// =============================================================================

// =============================================================================
//  NavBarItem: PRIVATE Methods
// =============================================================================

