//////////////////////////////////////////
//  File      : toolbutton.cpp		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#include <RectButton>

AmeRectButton::AmeRectButton(QWidget *parent)
        : QToolButton(parent)
{
        buttonStyle = IconButton;
        setWidgetSize(NormalSize);
        cmd = None;
}

AmeRectButton::AmeRectButton(const QString &text, const QString &tooltip, QWidget *parent)
        : QToolButton(parent)
{
        setText(text);
        setToolTip(tooltip);
        cmd = Custom;
        buttonStyle = TextButton;
        setWidgetSize(NormalSize);
}

AmeRectButton::AmeRectButton(const ButtonCmd c, const QString &tooltip, QWidget *parent)
        : QToolButton(parent)
{
        AmeSystemIcon ic((AmeSystemIcon::IconType)c);
        setIcon((QIcon)ic);
        cmd = c;
        setToolTip(tooltip);
        buttonStyle = IconButton;
        setWidgetSize(NormalSize);
}

AmeRectButton::AmeRectButton(const AmeSystemIcon &icon, const QString &tooltip, QWidget *parent)
        : QToolButton(parent)
{
        setIcon(icon);
        //cmd = icon.
        setToolTip(tooltip);
        buttonStyle = IconButton;
        setWidgetSize(NormalSize);
}

AmeRectButton::~AmeRectButton()
{
}

QSize AmeRectButton::sizeHint() const
{

        int w = 0, h = 0;

        if (wsize == NormalSize) {
                w = 28;
                h = 22;
        } else {
                w = 18;
                h = 16;
        }

        if (buttonStyle == TextButton) {
                QFontMetrics fm = fontMetrics();
                QSize textSize = fm.size(Qt::TextShowMnemonic, text());
                textSize.setWidth(textSize.width() + fm.width(QLatin1Char(' ')) * 2);
                w = textSize.width() + 4;
        }

        return QSize(w, h);
}

QSize AmeRectButton::minimumSizeHint() const
{
        return sizeHint();
}


void AmeRectButton::setWidgetSize(WSize s) {
        wsize = s;
        QFont f = font();
        f.setPointSize((int)wsize);
        setFont(f);
        QSize s1 = sizeHint();
        if (s1.isValid()) {
                resize(s1);
                setMinimumSize(s1);
                setMaximumSize(s1);
        }
        update();
}

void AmeRectButton::setButtonCmd(ButtonCmd c)
{
        AmeSystemIcon ic((AmeSystemIcon::IconType)c);
        setIcon((QIcon)ic);
        cmd = c;
        update();
}
