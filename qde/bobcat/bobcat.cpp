#include <QtGui>
#include "bobcat.h"
#include "macros.h"

class BobcatStylePlugin : public QStylePlugin
{
public:
        QStringList keys() const {
                return QStringList() << "Bobcat";
        }

        QStyle *create(const QString &key) {
                if (key == "bobcat")
                        return new Style();
                return 0;
        }
};

Q_EXPORT_PLUGIN2(Bobcat, BobcatStylePlugin)

// registerFunctions() and function array are borrowed from great Bespin style

#define N_PE 54
#define N_CE 50

static void (Style::*primitiveFunction[N_PE])(const QStyleOption *, QPainter *, const QWidget *) const;
static void (Style::*controlFunction[N_PE])(const QStyleOption *, QPainter *, const QWidget *) const;

void Style::registerFunctions()
{
        for (int i = 0; i < N_PE; ++i)
                primitiveFunction[i] = 0;
        for (int i = 0; i < N_CE; ++i)
                controlFunction[i] = 0;

        // buttons
        //registerPE(drawPushButton, PE_PanelButtonCommand);
        registerPE(skip, PE_PanelButtonBevel);

        // menus
        registerPE(drawMenuBar, PE_PanelMenuBar);
        registerCE(drawMenuBar, CE_MenuBarEmptyArea);
        registerCE(drawMenuBarItem, CE_MenuBarItem);
        registerCE(drawMenuItem, CE_MenuItem);

        // toolbars
        registerPE(drawToolBar, PE_PanelToolBar);
        registerCE(drawToolBar, CE_ToolBar);
        registerPE(skip, PE_IndicatorToolBarSeparator);
}

Style::Style() : QCleanlooksStyle()
{
        setObjectName(QLatin1String("Bobcat"));
        registerFunctions();
}

Style::~Style()
{

}

void Style::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
        if (element < N_PE && primitiveFunction[element]) {
                (this->*primitiveFunction[element])(option, painter, widget);

        } else
                QCleanlooksStyle::drawPrimitive(element, option, painter, widget);
}

void Style::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
        if (element < N_CE && controlFunction[element]) {
                (this->*controlFunction[element])(option, painter, widget);
        } else
                QCleanlooksStyle::drawControl(element, option, painter, widget);
}

#define maxFactor 100
QColor Style::mergeColors(const QColor &colorA, const QColor &colorB, int factor) const
{
    QColor tmp = colorA;
    tmp.setRed((tmp.red() * factor) / maxFactor + (colorB.red() * (maxFactor - factor)) / maxFactor);
    tmp.setGreen((tmp.green() * factor) / maxFactor + (colorB.green() * (maxFactor - factor)) / maxFactor);
    tmp.setBlue((tmp.blue() * factor) / maxFactor + (colorB.blue() * (maxFactor - factor)) / maxFactor);
    return tmp;
}

