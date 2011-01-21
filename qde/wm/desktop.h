///////////////////////////////////////////////////
//  File      : desktop      			         //
//  Originally Written by: g_cigala@virgilio.it  //
//  Modified by: ludmiloff@gmail.com			 //
//  Copyright : GPL                   			 //
///////////////////////////////////////////////////

// the desktop on the screen

#ifndef DESKTOP_H
#define DESKTOP_H

#include "defs.h"
#include <PixmapWidget>


class Adx;

class Desktop : public AmePixmapWidget
{
    Q_OBJECT

public:
    Desktop(Adx *a, QWidget *parent=0);
    ~Desktop();
    void init(int top_margin);

    void loadWallpaper(const QString &path);

public slots:
    void runMenu(QAction *);

private:
    QBoxLayout *layout;
    QMenu *menu;
    Adx *app;
};

#endif

