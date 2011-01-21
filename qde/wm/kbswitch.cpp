#include "kbswitch.h"
#include "adx.h"

KbSwitch::KbSwitch(Adx *a, Panel *p, QWidget *parent)
	: GenericButton(p, parent)
{
	app = a;
	setFixedWidth(42);
        if (readSettings()) {
                displayLayout();
        }
}

KbSwitch::~KbSwitch()
{
}

void KbSwitch::nextLayout()
{
	current ++;
	if (current >= layouts.size()) current = 0;
	displayLayout();
	saveSettings();
}

void KbSwitch::displayLayout()
{
	if (current >= layouts.size()) current = 0;
	KbLayout l = layouts.at(current);
        QString t = l.layout.left(3).toUpper();
	if (l.order > 1) t = t + QString("%1").arg(l.order);

        setText(t);
	//
	QString cmd = QString("setxkbmap -model %1 -layout %2").arg(keyboardModel).arg(l.layout);
	if (!l.variant.isEmpty())
		cmd = cmd + QString(" -variant %1").arg(l.variant);
	qDebug() << cmd;
	QProcess::startDetached(cmd);

}

bool KbSwitch::readSettings()
{
	layouts.clear();
	app->stg->sync();
	app->stg->beginGroup("Keyboard");
	keyboardModel = app->stg->value("model", "pc104").toString();
	int size = app->stg->beginReadArray("layouts");
	for (int i = 0; i < size; ++i) {
		app->stg->setArrayIndex(i);
		KbLayout l;
                l.name = app->stg->value("name", "").toString();
                l.layout = app->stg->value("layout", "").toString();
                l.variant = app->stg->value("variant", "").toString();
		l.order = app->stg->value("order", 0).toInt();
		layouts.append(l);
		qDebug() << "read " << l.name << l.layout << l.variant;
	}
	app->stg->endArray();
	//
	current = app->stg->value("current", 0).toInt();
	app->stg->endGroup(); //Keyboard

	int s = layouts.size();
	if (s > 0) {
		if (current >= s) current = 0;
		rebuildMenu();
		return true;
	} else {
                if (menu()) menu()->clear(); else setMenu(new QMenu());
		return false;
	}
}

bool KbSwitch::saveSettings()
{
	app->stg->beginGroup("Keyboard");
	app->stg->setValue("current", current);
	app->stg->endGroup();
	app->stg->sync();
	return true;
}

void KbSwitch::onPref()
{
	QProcess::startDetached("syspref.app -w 5");
}

void KbSwitch::onChangeLayout()
{
	QAction *action = qobject_cast<QAction *>(sender());
	current = action->data().toInt();
	displayLayout();
	saveSettings();
}

void KbSwitch::onChangeKbdModel(const QString &model)
{
	keyboardModel = model;
	QProcess::startDetached("setxkbmap -model " + keyboardModel);
}

void KbSwitch::rebuildMenu()
{
	if (menu()) {
		menu()->clear();
	}
	else
		setMenu(new QMenu());

	QActionGroup *g = new QActionGroup(this);

	for (int i = 0; i < layouts.size(); i++) {
		KbLayout l = layouts.at(i);
		QAction *it;
		if (l.variant.isEmpty())
			it = new QAction(l.name, this);
		else
			it = new QAction(l.name + " - " + l.variant, this);
		it->setCheckable(true);
		it->setData(i);
		if (current == i) it->setChecked(true);
		menu()->addAction(it);
		connect(it, SIGNAL(triggered()), this, SLOT(onChangeLayout()));
		g->addAction(it);
	}

	menu()->addSeparator();
	openPref = new QAction("Open Language settings ...", this);
	connect(openPref, SIGNAL(triggered()), this, SLOT(onPref()));
	menu()->addAction(openPref);
}
