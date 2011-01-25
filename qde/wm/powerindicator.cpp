#include "powerindicator.h"
#include "panel.h"
#include <PowerManager>

class PowerWidget : public QDialog
{

public:
    PowerWidget(QWidget *parent) : QDialog(parent), pwrMgr(new PowerManager(this))
    {
	QPalette p(palette());
	p.setColor(QPalette::Background, Qt::white);
	p.setColor(QPalette::Foreground, Qt::gray);
	setPalette(p);

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	batteryLevel = new QLabel(tr("Battery level: not defined"), this);
	layout->addWidget(batteryLevel);

	powerSupply = new QLabel(tr("Power source: not defined"), this);
	layout->addWidget(powerSupply);
    };

private:
    QLabel *batteryLevel;
    QLabel *powerSupply;
    PowerManager *pwrMgr;

friend class PowerIndicator;
};

PowerIndicator::PowerIndicator(Panel *p, QWidget *parent) : GenericButton(p, parent),
							    powerWdg(new PowerWidget(this))
{
	can_Activated = false;
	setFixedWidth(28);
	setIconSize(QSize(19, 18));
	setImages(QPixmap(":/default/battery0.png"), QPixmap(":/default/battery0-active.png"));

	connect(powerWdg->pwrMgr, SIGNAL(batteryLevel(int)), SLOT(updateBatteryLevel(int)));
	connect(powerWdg->pwrMgr, SIGNAL(batteryDischarging(bool)), SLOT(updatePowerSupplyStatus(bool)));
	powerWdg->pwrMgr->update();
}

PowerIndicator::~PowerIndicator()
{
}

void PowerIndicator::activate()
{
    if (!activated) {
	    panel->deactivateLast(this);
	    powerWdg->show();
	    powerWdg->move(this->mapToGlobal(QPoint(0, height())));
	    XRaiseWindow(display(), powerWdg->winId());
	    activated = true;
	    panel->setCurrentWidget(this, powerWdg->winId());
    }
}

void PowerIndicator::deactivate()
{
    if (activated) {
	    powerWdg->hide();
	    activated = false;
	    panel->unfocusChilds();
	    panel->clearCurrentWidget();
	    m_Focus = false;
	    update();
    }
}

void PowerIndicator::updatePowerSupplyStatus(bool batteryDischarging)
{
    if (batteryDischarging)
	powerWdg->powerSupply->setText(tr("Power source: Battery"));
    else
	powerWdg->powerSupply->setText(tr("Power source: Power adapter"));
}

void PowerIndicator::updateBatteryLevel(int level)
{
    QString levelStr = tr("Battery level: ") + QString::number(level) + '%';
    powerWdg->batteryLevel->setText(levelStr);
}



