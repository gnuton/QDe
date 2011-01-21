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
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QPainter>

#include "navbar.h"

// =============================================================================
//  NavBar: PRIVATE Class
// =============================================================================
class AmeNavBar::Private {
public:
	QVBoxLayout *layout;

	// Colors
	QColor colorBackground;
	QColor colorSelection;

	// Current Selection
	AmeNavBarGroup *groupSelected;
	AmeNavBarItem *itemSelected;
};

// =============================================================================
//  NavBar: PUBLIC Constructors/Destructors
// =============================================================================
AmeNavBar::AmeNavBar (QWidget *parent)
	: QWidget(parent), d(new AmeNavBar::Private)
{
	// Initialize Members
	d->layout = new QVBoxLayout;
	d->itemSelected = NULL;
	d->groupSelected = NULL;

	// Setup Colors
	d->colorBackground = QColor(0xdf, 0xe4, 0xea);
	d->colorSelection = QColor(0xa6, 0xa6, 0xa6);

	// Setup Layout
	d->layout->addStretch(2);
	setLayout(d->layout);
}

AmeNavBar::~AmeNavBar() {
	delete d;
	d = NULL;
}

// =============================================================================
//  NavBar: PUBLIC Methods - Groups Related
// =============================================================================
void AmeNavBar::addGroup (AmeNavBarGroup *group) {
	// Set Font
	group->setTitleColor(QColor(0x65, 0x71, 0x80));

	// Add Events
	connect(group, SIGNAL(selected(AmeNavBarGroup *, AmeNavBarItem *)), 
			this, SLOT(onItemSelected(AmeNavBarGroup *, AmeNavBarItem *)));

	// Add to Layout
	d->layout->insertWidget(d->layout->count() - 1, group);
}

AmeNavBarGroup *AmeNavBar::addGroup (const QString& title) {
	AmeNavBarGroup *group = new AmeNavBarGroup(title);
	addGroup(group);
	return(group);
}

bool AmeNavBar::containsGroup (AmeNavBarGroup *group) {
	int itemCount = d->layout->count();

	for (int i = 0; i < itemCount; ++i) {
		QWidget *widget = d->layout->itemAt(i)->widget();
		if (widget == group) return(true);
	}
	
	return(false);
}

// =============================================================================
//  NavBar: PUBLIC Methods - Items Related
// =============================================================================
void AmeNavBar::addItem (AmeNavBarGroup *group, AmeNavBarItem *item) {
	group->addItem(item);
}

AmeNavBarItem *AmeNavBar::addItem (AmeNavBarGroup *group, const QString &text, const QString &link) {
	return(group->addItem(text, link));
}

AmeNavBarItem *AmeNavBar::addItem (AmeNavBarGroup *group,
		const QPixmap &icon, const QString &text, const QString &link) {
	return(group->addItem(icon, text, link));
}


// =============================================================================
//  NavBar: PUBLIC Get Properties
// =============================================================================
QColor AmeNavBar::backgroundColor (void) const {
	return(d->colorBackground);
}

QColor AmeNavBar::selectionColor (void) const {
	return(d->colorSelection);
}

// =============================================================================
//  NavBar: PUBLIC Set Properties
// =============================================================================
void AmeNavBar::setBackgroundColor (const QColor& bgColor) {
	d->colorBackground = bgColor;
}

void AmeNavBar::setSelectionColor (const QColor& selectionColor) {
	d->colorSelection = selectionColor;
}

// =============================================================================
//  NavBar: PUBLIC Slots
// =============================================================================

// =============================================================================
//  NavBar: PROTECTED Methods
// =============================================================================
void AmeNavBar::paintEvent (QPaintEvent *event) {
	QWidget::paintEvent(event);

	// Recall Update() if painter area is not complete!
	if (event->rect().x() > 0 || event->rect().y() > 0)
		update();

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.fillRect(event->rect(), d->colorBackground);

	// Draw Selection
	if (d->groupSelected != NULL && d->groupSelected->isExpanded() && d->itemSelected != NULL) {
		QPoint pos = d->groupSelected->pos() + d->itemSelected->pos();
		int width = geometry().width();

		int r = d->colorSelection.red();
		int g = d->colorSelection.green();
		int b = d->colorSelection.blue();

		p.fillRect(0, pos.y() - 1, width, 1, QColor(r - 0x26, g - 0x26, b - 0x26));

		QLinearGradient linearGrad(QPointF(0, pos.y()), QPointF(0, pos.y() + d->itemSelected->height()));
		linearGrad.setColorAt(0, d->colorSelection);
		linearGrad.setColorAt(1, QColor(r - 0x3b, g - 0x3b, b - 0x3b));
		p.fillRect(0, pos.y(), width, d->itemSelected->height(), linearGrad);
	}

	p.end();
}

// =============================================================================
//  NavBar: PRIVATE Slots
// =============================================================================
void AmeNavBar::onItemSelected (AmeNavBarGroup *group, AmeNavBarItem *item) {
	if (d->itemSelected != NULL && d->itemSelected != item) {
		d->itemSelected->setFont(item->font());
		d->itemSelected->setTextColor(Qt::black);
		d->itemSelected->unselect();
	}

	d->groupSelected = group;
	d->itemSelected = item;

	QFont font = d->itemSelected->font();
	font.setPixelSize(9);
	font.setBold(true);
	d->itemSelected->setFont(font);
	d->itemSelected->setTextColor(Qt::white);

	update();
}

void AmeNavBar::onGroupExpanded (AmeNavBarGroup *group) {
	Q_UNUSED(group)

	update();
}

// =============================================================================
//  NavBar: PRIVATE Methods
// =============================================================================


