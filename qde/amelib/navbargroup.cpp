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
#include <QVBoxLayout>
#include <QLabel>


#include <ClickableLabel>
#include <NavBarGroup>

// =============================================================================
//  NavBarGroup: PRIVATE Class
// =============================================================================
class AmeNavBarGroup::Private {
	public:
		QList<AmeNavBarItem *> listItems;
		AmeClickableLabel *labelTitle;
		QVBoxLayout *layout;
		bool isExpanded;

	public:
		void initialize (AmeNavBarGroup *group);
};

void AmeNavBarGroup::Private::initialize (AmeNavBarGroup *group) {
	// Initialize Members
	layout = new QVBoxLayout;
	labelTitle = new AmeClickableLabel;

	// Setup Expanded Flag
	isExpanded = true;

	// Add Layout Items
	layout->addWidget(labelTitle);

	// Setup Label Title
	QFont font = labelTitle->font();
	font.setBold(true);
	font.setPixelSize(10);
	labelTitle->setFont(font);
	labelTitle->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

	// Setup Layout
	layout->setSpacing(1);
	layout->setContentsMargins(0, 0, 0, 0);
	group->setLayout(layout);
}

// =============================================================================
//  NavBarGroup: PUBLIC Constructors/Destructors
// =============================================================================
AmeNavBarGroup::AmeNavBarGroup (QWidget *parent)
	: QWidget(parent), d(new AmeNavBarGroup::Private)
{
	d->initialize(this);

	// Add Events
	connect(d->labelTitle, SIGNAL(clicked()), this, SLOT(onTitleClicked()));
}

AmeNavBarGroup::AmeNavBarGroup (const QString& title, QWidget *parent)
	: QWidget(parent), d(new AmeNavBarGroup::Private)
{
	d->initialize(this);
	d->labelTitle->setText(title);

	// Add Events
	connect(d->labelTitle, SIGNAL(clicked()), this, SLOT(onTitleClicked()));
}

AmeNavBarGroup::~AmeNavBarGroup() {
	delete d;
	d = NULL;
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods - Add Item
// =============================================================================
void AmeNavBarGroup::addItem (AmeNavBarItem *item) {
	// Insert Item Space
	item->insertSpacing(0, 10);

	// Insert Item into Item List
	d->listItems.append(item);

	// Add Item to Layout
	d->layout->addWidget(item);

	// Add Event
	connect(item, SIGNAL(selected(AmeSelectableWidget *)), 
			this, SLOT(onItemSelected(AmeSelectableWidget *)));
}

void AmeNavBarGroup::addItem (AmeNavBarItem *item, int index) {
	// Insert Item Space
	item->insertSpacing(0, 20);

	// Insert Item into Item List
	d->listItems.insert(index, item);

	// Add Item to Layout
	d->layout->insertWidget(index, item);

	// Add Event
	connect(item, SIGNAL(selected(AmeSelectableWidget *)), 
			this, SLOT(onItemSelected(AmeSelectableWidget *)));
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods - Create and Add Item
// =============================================================================
AmeNavBarItem *AmeNavBarGroup::addItem (const QString &text, const QString &link) {
	AmeNavBarItem *item = new AmeNavBarItem(text, link);
	addItem(item);
	return(item);
}

AmeNavBarItem *AmeNavBarGroup::addItem (const QPixmap &icon, const QString &text, const QString &link) {
	AmeNavBarItem *item = new AmeNavBarItem(icon, text, link);
	addItem(item);
	return(item);
}

AmeNavBarItem *AmeNavBarGroup::addItem (const QPixmap &icon, const QString &text, const QString &link, int index) {
	AmeNavBarItem *item = new AmeNavBarItem(icon, text, link);
	addItem(item, index);	
	return(item);
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods
// =============================================================================
bool AmeNavBarGroup::containsItem (AmeNavBarItem *item) {
	return(d->listItems.contains(item));
}

// =============================================================================
//  NavBarGroup: PUBLIC Get Properties
// =============================================================================
QString AmeNavBarGroup::title (void) const {
	return(d->labelTitle->text());
}

bool AmeNavBarGroup::isExpanded (void) const {
	return(d->isExpanded);
}

// =============================================================================
//  NavBarGroup: PUBLIC Set Properties
// =============================================================================
void AmeNavBarGroup::setTitle (const QString& title) {
	d->labelTitle->setText(title);
}

void AmeNavBarGroup::setTitleColor (const QColor& color) {
	QPalette palette = d->labelTitle->palette();
	palette.setColor(QPalette::WindowText, color);
	d->labelTitle->setPalette(palette);
}

// =============================================================================
//  NavBarGroup: PUBLIC Slots
// =============================================================================
void AmeNavBarGroup::expand (bool expand) {
	if (d->isExpanded == expand) return;

	if (expand) {
		foreach (AmeNavBarItem *item, d->listItems) {
			d->layout->addWidget(item);
			item->show();
		}
	} else {
		foreach (AmeNavBarItem *item, d->listItems) {
			d->layout->removeWidget(item);
			item->hide();
		}
	}

	d->isExpanded = expand;

	// Raise Expanded Event
	emit expanded(this);
}

// =============================================================================
//  NavBarGroup: PROTECTED Methods
// =============================================================================

// =============================================================================
//  NavBarGroup: PRIVATE Slots
// =============================================================================
void AmeNavBarGroup::onItemSelected (AmeSelectableWidget *item) {
	emit selected(this, (AmeNavBarItem *) item);
}

void AmeNavBarGroup::onTitleClicked (void) {
	expand(!d->isExpanded);
}

// =============================================================================
//  NavBarGroup: PRIVATE Methods
// =============================================================================

