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
#include <QMouseEvent>

#include <SelectableWidget>

// =============================================================================
//  SelectableWidget: PRIVATE Class
// =============================================================================
class AmeSelectableWidget::Private {
	public:
		bool isSelected;
};

// =============================================================================
//  SelectableWidget: PUBLIC Constructors/Destructors
// =============================================================================
AmeSelectableWidget::AmeSelectableWidget (QWidget *parent)
	: QWidget(parent), d(new AmeSelectableWidget::Private)
{
	d->isSelected = false;
}

AmeSelectableWidget::~AmeSelectableWidget() {
	delete d;
	d = NULL;
}

// =============================================================================
//  SelectableWidget: PUBLIC Methods
// =============================================================================

// =============================================================================
//  SelectableWidget: PUBLIC Get Properties
// =============================================================================
bool AmeSelectableWidget::isSelected (void) const {
	return(d->isSelected);
}

// =============================================================================
//  SelectableWidget: PUBLIC Set Properties
// =============================================================================

// =============================================================================
//  SelectableWidget: PUBLIC Slots
// =============================================================================
void AmeSelectableWidget::select (void) {
	select(true);
}

void AmeSelectableWidget::select (bool isSelected) {
	if (d->isSelected == isSelected) return;

	d->isSelected = isSelected;

	// if is selected raise event
	if (isSelected) {
		emit selected();
		emit selected(this);
	}

	update();
}

void AmeSelectableWidget::unselect (void) {
	select(false);
}

// =============================================================================
//  SelectableWidget: PROTECTED Methods
// =============================================================================
void AmeSelectableWidget::mouseReleaseEvent (QMouseEvent *event) {
	QWidget::mouseReleaseEvent(event);

	// Left Click, Select/Unselect Item
	if (event->button() == Qt::LeftButton)
		select(!d->isSelected);
}

// =============================================================================
//  SelectableWidget: PRIVATE Slots
// =============================================================================

// =============================================================================
//  SelectableWidget: PRIVATE Methods
// =============================================================================

