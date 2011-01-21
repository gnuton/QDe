#include <DesktopFile>
#include "menu.h"
#include "panel.h"

MenuAction::MenuAction(const QString &text, const QString &comment, QObject *parent)
	: QAction(text, parent)
{
	Q_UNUSED(comment);	
}

MenuAction::MenuAction(const QIcon &icon, const QString &text, const QString &comment, QObject *parent)
	: QAction(icon, text, parent)
{
	Q_UNUSED(comment);
}

void MenuAction::setCommand(const QString &cmd, bool runAsRoot)
{
	Q_UNUSED(runAsRoot);
	m_cmd = cmd;
	int dot = m_cmd.indexOf("%");
	while (dot > 0) {
		m_cmd[dot] = QChar(' ');
		if (dot < cmd.length() - 1)
			m_cmd[dot+1] = QChar(' ');
		dot = m_cmd.indexOf("%");
	}
	m_cmd = m_cmd.trimmed();
}

void MenuAction::runCommand()
{
	//qDebug() << "run command " << m_cmd;
	QProcess::startDetached(m_cmd);
}

bool compareMnuAct(MenuAction *a1, MenuAction *a2)
{
	return a1->text() < a2->text();
}

MenuButton::MenuButton(Panel *p, QWidget *parent)
	: GenericButton(p, parent)
{
	setText("Applications");

	m_Catalog["Utility"] = new QIcon("util.png");
	m_Catalog["Office"] = new QIcon("office.png");
	m_Catalog["Network"] = new QIcon("network.png");
	m_Catalog["Graphics"] = new QIcon("graphics.png");
	m_Catalog["AudioVideo"] = new QIcon("multimedia.png");
	m_Catalog["Game"] = new QIcon("games.png");
	m_Catalog["System"] = new QIcon("system.png");
	m_Catalog["Development"] = new QIcon("devel.png");
	m_Catalog["Other"] = new QIcon("others.png");

	buildMenu();
	
	connect(this, SIGNAL(clicked()), this, SLOT(showMenu()));
}

MenuButton::~MenuButton()
{
}

void MenuButton::readSystemMenu(const QString &path)
{
	bool found;
	QDir dir;	
	if (!dir.setCurrent(path)) 
		return;
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); i++) {
		QFileInfo fi = list.at(i);
		if ((fi.isDir()) && !(fi.fileName().left(1) == "."))  {			
			readSystemMenu(path+fi.fileName()+"/");
		}
		else if (AmeDesktopFile::isDesktopFile(path+fi.fileName())) {
			AmeDesktopFile *desktop = new AmeDesktopFile(path+fi.fileName());
			QString title = desktop->readName();
			if (!title.isEmpty() && desktop->isVisible()) {
				QStringList cat = desktop->readCategories();
				QString execCmd = desktop->readCommand();
				//QString data = desktop->value("Categories", QVariant("")).toString();

				found = false;
				for (int c = 0; c < cat.size(); c++) {
					if (m_Catalog.contains(cat[c])) {
						QList <MenuAction *> items = m_Menu.values(cat[c]);
						for (int i=0;i<items.size();i++) {
							if (items[i]->text() == title) {
								found = true;
								break;
							}
						}
						if (!found){
							MenuAction *act = new MenuAction(title, "", this);
							act->setCommand(execCmd);
							connect(act, SIGNAL(triggered(bool)), act, SLOT(runCommand()));
							m_Menu.insert(cat[c], act);
							found = true;						
						}						
						break;
					}
				}
				if (!found) {
					QList <MenuAction *> items = m_Menu.values("Other");
					for (int i=0;i<items.size();i++) {
						if (items[i]->text() == title) {
							found = true;
							break;
						}
					}
					if (!found) {
						MenuAction *act = new MenuAction(title, "", this);
						act->setCommand(execCmd);
						connect(act, SIGNAL(triggered(bool)), act, SLOT(runCommand()));
						m_Menu.insert("Other", act);
					}
				}	
			}
		}
	}		
}

void MenuButton::buildMenu()
{
	QMenu *menu = new QMenu(this);

	// TODO: read path value from system settings
	readSystemMenu("/usr/share/applications/");

	QList <QString> cat = m_Menu.uniqueKeys();
	for (int k = 0; k < cat.size(); k++) {
		QMenu *submenu = menu->addMenu(cat[k]);

		QList <MenuAction *> items = m_Menu.values(cat[k]);
		qSort(items.begin(), items.end(), compareMnuAct);
		
		for (int i = 0; i < items.size(); i++) {
			submenu->addAction(items[i]);
		}
	}	

	setMenu(menu);
}
