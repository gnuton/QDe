////////////////////////////////////////
//  File      : systray.h             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////


#ifndef __SYSTRAY_H
#define __SYSTRAY_H

#include "defs.h"


class XContainer : public QX11EmbedContainer
{
        Q_OBJECT
public:
        XContainer(QWidget *parent = 0);
        ~XContainer();

signals:
        void containerDeleted(WId);
};

class Systray : public QWidget
{
        Q_OBJECT

public:
        Systray(QWidget *parent=0);
        ~Systray();

        void addEmbed(Window);
        void removeEmbed(Window);


private:
        int embedSize;
        QHash<int, XContainer* > embed; // mapping Client win_id with QX11EmbedContainer
                                                // (key=client id, value=Container)
        QHBoxLayout *layout;
        XContainer *container;
};

#endif
