#include <GradientButton>
#include <QtGlobal>

AmeGradientButton::AmeGradientButton(QWidget *parent)
        : QToolButton(parent)
{
        cmd = None;
        w1 = 22;
        setGroupPosition(LeftButton);
}


AmeGradientButton::AmeGradientButton(const ButtonCmd c, QWidget *parent)
        : QToolButton(parent)
{
        AmeSystemIcon ic((AmeSystemIcon::IconType)c);
        setIcon(QIcon(ic));
        cmd = c;
        w1 = 22;
        setGroupPosition(LeftButton);
}

AmeGradientButton::~AmeGradientButton()
{
}


QSize AmeGradientButton::sizeHint() const
{
        return QSize(w1, 20);
}

QSize AmeGradientButton::minimumSizeHint() const
{
        return sizeHint();
}

void AmeGradientButton::restrictSize()
{
        QSize s1 = sizeHint();
        if (s1.isValid()) {
                resize(s1);
                setMinimumSize(s1);
                setMaximumSize(s1);
        }
        update();
}

void AmeGradientButton::setGroupPosition(GroupPosition g)
{
        gPos = g;
        if (gPos == MidButton) w1 = 21; else w1 = 22;
        restrictSize();
        update();
}

void AmeGradientButton::paintEvent(QPaintEvent *)
{
        QPainter painter(this);

        QColor outlineColor1(0x7e, 0x7e, 0x7e);
        QColor outlineColor2(0x97, 0x97, 0x97);

        QColor topColor, bottomColor;
        if (isDown()) {
                topColor = QColor(0xa3, 0xa3, 0xa3);
                bottomColor = QColor(0x95, 0x95, 0x95);
        } else {
                topColor = QColor(0xfb, 0xfb, 0xfb);
                bottomColor = QColor(0xee, 0xee, 0xee);
        }

        QRect r = painter.window();

        int left, right, h, w, x1, x2;
        h = (height() - 2) / 2;
        x1 = 1; x2 = 0;

        switch (gPos) {
                case LeftButton : {
                        w = width() - 1;
                        left=r.left() + 1;
                        right=r.right();
                        x1 = 2;
                        break;
                }
                case MidButton: {
                        w = width();
                        left = r.left();
                        right = r.right();
                        break;
                }
                case RightButton : {
                        w = width() - 2;
                        left = r.left();
                        right = r.right();
                        x2 = -1;
                        break;
                }
        }

        painter.setPen(QPen(outlineColor1, 0));
        painter.drawLine(QPoint(left, r.top()), QPoint(right, r.top()));


        int y1 = r.top() + 1;
        painter.setPen(QPen(outlineColor2, 0));
        painter.drawLine(QPoint(left, y1), QPoint(left, r.bottom()));
        painter.drawLine(QPoint(left, r.bottom()), QPoint(right, r.bottom()));

        if (gPos == RightButton)
                painter.drawLine(QPoint(right, y1), QPoint(right, r.bottom()));

        left ++;
        painter.fillRect(left, 1, w, h, topColor);
        painter.fillRect(left, h + 1, w, h, bottomColor);

        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.rect.adjust(x1, 1, x2, -1);

        QPixmap pix = icon().pixmap(iconSize(),
                isEnabled() ? QIcon::Normal : QIcon::Disabled,
                isDown() ? QIcon::On : QIcon::Off);

        int pw = pix.width();
        int ph = pix.height();

        QPoint point(option.rect.x() + option.rect.width() / 2 - pw / 2,
                     option.rect.y() + option.rect.height() / 2 - ph / 2);

        painter.drawPixmap(style()->visualPos(layoutDirection(), option.rect, point), pix);

        if (hasFocus()) {
                option.backgroundColor = palette().color(QPalette::Background);
                style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
        }

}

void AmeGradientButton::setButtonCmd(ButtonCmd c)
{
        AmeSystemIcon ic((AmeSystemIcon::IconType)c);
        setIcon((QIcon)ic);
        cmd = c;
        update();
}
