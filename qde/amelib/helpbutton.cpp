#include <HelpButton>

AmeHelpButton::AmeHelpButton(QWidget *parent)
        : QToolButton(parent),
          helpUrl("")
{
        setIcon(QIcon(":/sysicons/helpbtn.png"));
        restrictSize();
}


AmeHelpButton::AmeHelpButton(const QString &url, QWidget *parent)
        : QToolButton(parent),
          helpUrl(url)
{
        setIcon(QIcon(":/sysicons/helpbtn.png"));
        restrictSize();
}

AmeHelpButton::~AmeHelpButton()
{
}

QSize AmeHelpButton::sizeHint() const
{
        return QSize(23, 23);
}

QSize AmeHelpButton::minimumSizeHint() const
{
        return sizeHint();
}

void AmeHelpButton::restrictSize()
{
        QSize s1 = sizeHint();
        if (s1.isValid()) {
                resize(s1);
                setMinimumSize(s1);
                setMaximumSize(s1);
        }
        update();
}

void AmeHelpButton::paintEvent(QPaintEvent *)
{
        QPainter painter(this);

        QPixmap pix;

        if (isDown())
                pix = icon().pixmap(iconSize(),
                        isEnabled() ? QIcon::Selected : QIcon::Disabled,
                        isDown() ? QIcon::On : QIcon::Off);
        else
                pix = icon().pixmap(iconSize(),
                        isEnabled() ? QIcon::Normal : QIcon::Disabled,
                        isDown() ? QIcon::On : QIcon::Off);


        painter.drawPixmap(0, 0, pix);
}
