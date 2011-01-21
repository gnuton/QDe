#include "adx.h"
#include "panel.h"
#include "showdesktop.h"

ShowDesktop::ShowDesktop(Adx *a, Panel *p, QWidget *parent)
	: GenericButton(p, parent)
{
	app = a;
	can_Activated = false;
	m_iconState = False;

	setIconSize(QSize(17, 16));
	setImages(QPixmap(":/default/show-desk.png"), QPixmap(":/default/show-desk-active.png"));

	//connect(this, SIGNAL(clicked()), this, SLOT(onIconify()) );
}

ShowDesktop::~ShowDesktop(void)
{
}

void ShowDesktop::iconify()
{
	if (!m_iconState)
		app->minimizeAll();
	else
		app->showAll();
	m_iconState = !m_iconState;
}


void ShowDesktop::deactivate()
{
	if (m_iconState) {
		m_Focus = false;
		activated = false;
		m_iconState = False;
		update();
	}
}

void ShowDesktop::mousePressEvent(QMouseEvent *)
{
	panel->grabFocus();
	if (!activated) {
		//panel->deactivateLast(this);
		//panel->setCurrent(this, this->winId());
		iconify();
		activated = true;
		m_Focus = true;
	} else {
		//panel->clearCurrent();
		activated = false;
		m_Focus = false;
		iconify();
	}
	panel->unfocusChilds();
	update();
}

