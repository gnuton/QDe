#include "alttab.h"
#include "adx.h"

ClientIcon::ClientIcon(Client *client, int scale, QWidget *parent)
	: AmeIcon(client->appName, client->appIcon, scale, parent)
{
	m_Client = client;

}

ClientIcon::~ClientIcon()
{
}

AltTabDlg::AltTabDlg(Adx *a, Client *active, ClientList *clients, QWidget *parent)
	: QDialog(parent)
{
	app = a;
	int h = 0;
	int w = a->desktop->width();
	int numIcons = clients->size();
	int s = w / (numIcons * 2);
	
	int iconSize = 0;
	if (s >= 128) {
		iconSize = AmeIcon::normal;
	}
	else if (s >= 64 && s < 128) {
		iconSize = AmeIcon::normal;
	}
	else if (s >= 48 && s < 64) {
		iconSize = AmeIcon::medium;
	}
	else if (s >= 40 && s < 48) {
		iconSize = AmeIcon::average;
	}
	else if (s < 40) {
		iconSize = AmeIcon::small;
	}
	
	//qDebug() << "ICON SIZE=" << iconSize;
	
	layout = new QHBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(0, 12, 0, 12);
	
	layout->addStretch();
	// add current active client first
	if (active != NULL) {
		ClientIcon *first = new ClientIcon(active, iconSize);
		icons << first;
		h = first->height();
		layout->addWidget(first);
	}
	
	Client *c;

	foreach (c, *clients) {
		if (active != NULL && active == c) continue;
		ClientIcon *icon = new ClientIcon(c, iconSize);
		icons << icon;
		h = icon->height();
		layout->addWidget(icon);
	}
	
	if (icons.size() > 1)
		current = 1;
	else
		current = 0;

	ClientIcon *icon = icons.at(current);
	icon->drawBorder(true);
	
	layout->addStretch();
	
	setLayout(layout);
	resize(w, h+24);
	move(0, a->desktop->height()/2 - height() / 2);
	
	XUngrabKeyboard(display(), CurrentTime);
	XSetInputFocus(display(), winId(), None, CurrentTime);
	XRaiseWindow(display(), winId());
}

AltTabDlg::~AltTabDlg()
{
}

Client *AltTabDlg::selectedIcon()
{
	return icons.at(current)->m_Client;
}

void AltTabDlg::reject()
{
	app->m_Process = Adx::process_Normal;
	QDialog::reject();
}

void AltTabDlg::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Tab) {
		icons.at(current)->drawBorder(false);
		current ++;
		if (current >= icons.size()) current = 0;
		icons.at(current)->drawBorder(true);
		e->accept();
	} else 
		QDialog::keyPressEvent(e);
}

/* FIXME: NOT NEEDED
void AltTabDlg::keyReleaseEvent(QKeyEvent *e)
{
	//qDebug() << "KEY RELEASED (ALT_TAB_DLG)";
	QDialog::keyReleaseEvent(e);
}
*/
