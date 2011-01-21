#include <QtGui>
#include "bobcat.h"
#include "macros.h"

void Style::drawToolBar(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
        QRect r = option->rect;
        QMainWindow *p = (QMainWindow *) widget->parentWidget();
        QColor top;
        QColor bottom(0x96, 0x96, 0x96);

        painter->save();
        if (p->menuWidget() != 0L) {
                //qDebug() << "window has menu";
                top = QColor(0xa2, 0xa2, 0xa2);
        } else {
                //qDebug() << "menuless window";
                top = QColor(0xb3, 0xb3, 0xb3);
        }

        QLinearGradient gradient(QPointF(0, r.top()), QPointF(0, r.top()+widget->height()));
        gradient.setColorAt(0, top);
        gradient.setColorAt(1, bottom);
        painter->fillRect(r, gradient);

        QColor border = bottom.darker(200);
        painter->setPen(QPen(border, 0));
        painter->drawLine(QPoint(r.left(), r.bottom()), QPoint(r.right(), r.bottom()));
        painter->restore();

}
