#include <QtGui>
#include "bobcat.h"
#include "macros.h"

void Style::drawPushButton(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
        bool isDown = false;
        bool isDefault = (option->state & State_Sunken) || (option->state & State_On);
        bool isEnabled = (option->state & State_Enabled);

        QRect rect = option->rect;

        painter->drawPixmap(rect, QPixmap(":/pushbuttons/bp-left.png"));
}
