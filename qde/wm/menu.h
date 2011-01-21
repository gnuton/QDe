#ifndef __MENU_H
#define __MENU_H

#include "defs.h"
#include "button.h"

class Panel;

class MenuAction : public QAction
{
	Q_OBJECT
public:
	MenuAction(const QString &text, const QString &comment, QObject *parent = 0);
	MenuAction(const QIcon &icon, const QString &text, const QString &comment, QObject *parent = 0);
	 
	void setCommand(const QString &cmd, bool runAsRoot = false);

public slots:
	void runCommand();

private:
	QString m_cmd;
	bool m_runAsRoot;
};

class MenuButton : public GenericButton
{
public:
	explicit MenuButton(Panel *p, QWidget *parent=0);
	virtual ~MenuButton();


protected:
	QMap <QString, QIcon* > m_Catalog;
	QMultiMap <QString, MenuAction*> m_Menu;

	void buildMenu();
	void readSystemMenu(const QString &path);
};


#endif
