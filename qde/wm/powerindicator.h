#ifndef POWERINDICATOR_H
#define POWERINDICATOR_H
#include "button.h"

class Panel;
class PowerWidget;

class PowerIndicator : public GenericButton
{
	Q_OBJECT
public:
	PowerIndicator(Panel *p, QWidget *parent = 0);
	~PowerIndicator();

	virtual void activate();
	virtual void deactivate();

private slots:
	void updatePowerSupplyStatus(bool);
	void updateBatteryLevel(int);

private:
	PowerWidget *powerWdg;
};

#endif // POWERINDICATOR_H
