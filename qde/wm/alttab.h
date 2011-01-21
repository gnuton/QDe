#ifndef __ALTTAB_H
#define __ALTTAB_H

#include <Icon>
#include "client.h"

class Adx;

class ClientIcon : public AmeIcon
{
public:
	ClientIcon(Client *client, int scale, QWidget *parent = 0);
	~ClientIcon();

	Client *m_Client;
};

class AltTabDlg : public QDialog
{
	Q_OBJECT
public:
	AltTabDlg(Adx *a, Client *active, ClientList *clients, QWidget *parent = 0);
	~AltTabDlg();

	Client *selectedIcon();

	int current;
	QList <ClientIcon *> icons;
	QHBoxLayout *layout;

public slots:
	void reject();

protected:
	virtual void keyPressEvent(QKeyEvent *);
	//virtual void keyReleaseEvent(QKeyEvent *);

	Adx *app;
};

#endif

