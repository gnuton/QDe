#ifndef __BOBCAT_H
#define __BOBCAT_H

#include <QtGui>
#include <QCleanlooksStyle>

// from windows style
#define windowsItemFrame                2 // menu item frame width
#define windowsSepHeight                6 // separator item height
#define windowsItemHMargin              3 // menu item hor text margin
#define windowsItemVMargin              8 // menu item ver text margin
#define windowsArrowHMargin             6 // arrow horizontal margin
#define windowsTabSpacing               12 // space between text and tab
#define windowsCheckMarkHMargin         2 // horiz. margins of check mark
#define windowsRightBorder              15 // right border on windows
#define windowsCheckMarkWidth           12 // checkmarks width on windows

class
#ifdef Q_WS_WIN
Q_GUI_EXPORT
#endif

Style : public QCleanlooksStyle
{
        Q_OBJECT

public:
        Style();
        ~Style();

        void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                           QPainter *painter, const QWidget *widget) const;

        void drawControl(ControlElement element, const QStyleOption *option,
                          QPainter *painter, const QWidget *widget) const;

protected:
        // buttons
        void drawPushButton(const QStyleOption *, QPainter *, const QWidget *) const;

        // menus
        void drawMenuBar(const QStyleOption *, QPainter *, const QWidget *) const;
        void drawMenuBarItem(const QStyleOption *, QPainter *, const QWidget *) const;
        void drawMenuItem(const QStyleOption *, QPainter *, const QWidget *) const;

        // toolbars
        void drawToolBar(const QStyleOption *, QPainter *, const QWidget *) const;

        // skip function
        void skip(const QStyleOption*, QPainter*, const QWidget*) const {}

private:
        void registerFunctions();
        QColor mergeColors(const QColor &colorA, const QColor &colorB, int factor = 50) const;

};


#endif
