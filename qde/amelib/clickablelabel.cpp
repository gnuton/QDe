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

// Quartica Headers
#include <ClickableLabel>

// =============================================================================
//  ClickableLabel: PRIVATE Class
// =============================================================================

// =============================================================================
//  ClickableLabel: PUBLIC Constructors/Destructors
// =============================================================================
AmeClickableLabel::AmeClickableLabel (QWidget *parent, Qt::WindowFlags f)
	: QLabel(parent, f)
{
}

AmeClickableLabel::AmeClickableLabel (const QString& text, QWidget *parent, Qt::WindowFlags f)
	: QLabel(text, parent, f)
{
}

AmeClickableLabel::~AmeClickableLabel() {
}

// =============================================================================
//  ClickableLabel: PUBLIC Methods
// =============================================================================

// =============================================================================
//  ClickableLabel: PUBLIC Get Properties
// =============================================================================

// =============================================================================
//  ClickableLabel: PUBLIC Set Properties
// =============================================================================

// =============================================================================
//  ClickableLabel: PUBLIC Slots
// =============================================================================

// =============================================================================
//  ClickableLabel: PROTECTED Methods
// =============================================================================
void AmeClickableLabel::mouseReleaseEvent (QMouseEvent *event) {
	QLabel::mouseReleaseEvent(event);

	// Accept Event
	event->setAccepted(true);

	// Raise Clicked Event
	emit clicked();
	emit clicked(this);	
}

// =============================================================================
//  ClickableLabel: PRIVATE Slots
// =============================================================================

// =============================================================================
//  ClickableLabel: PRIVATE Methods
// =============================================================================

