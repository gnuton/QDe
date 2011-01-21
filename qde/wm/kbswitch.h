#ifndef __KBSWITCH_H
#define __KBSWITCH_H

#include "button.h"

class Adx;
class Panel;

struct KbLayout
{
	QString name;
	QString layout, variant;
	int order;
};

class KbSwitch : public GenericButton
{
	Q_OBJECT

public:
	KbSwitch(Adx *a, Panel *p, QWidget *parent = 0);
	virtual ~KbSwitch();

	void nextLayout();
        void displayLayout();

	bool readSettings();
	bool saveSettings();

	void rebuildMenu();

	int current;
	QList <KbLayout> layouts;
	QString keyboardModel;

public slots:
	void onPref();
	void onChangeLayout();
	void onChangeKbdModel(const QString &model);

protected:
	Adx *app;
	QAction *openPref;
};

#endif // __KBSWITCH_H
