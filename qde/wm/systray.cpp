//////////////////////////////////////////
// File : systray.cpp                   //
// Written by: g_cigala@virgilio.it     //
// Modified by: ludmiloff@gmail.com     //
// Copyright : GPL                      //
//////////////////////////////////////////

#include "atoms.h"
#include "systray.h"
#include "panel.h"

/* defined in the systray spec */
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2


XContainer::XContainer(QWidget *parent)
        : QX11EmbedContainer(parent)
{
        // just in case QX11EmbedContainer get fixed
        connect(this, SIGNAL(clientClosed()), SLOT(deleteLater()));
}

XContainer::~XContainer()
{
        emit containerDeleted(clientWinId());
}


Systray::Systray(QWidget *parent) : QWidget(parent)
{
        embedSize = TOP_PANEL_HEIGHT-1;
        layout = new QHBoxLayout();
        layout->setAlignment(Qt::AlignRight);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(1);
        setLayout(layout);
        resize(embedSize, embedSize);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        // Freedesktop.org System Tray support
        char name[20] = {0};
        qsnprintf(name, 20, "_NET_SYSTEM_TRAY_S%d", DefaultScreen(display()));
        Atom net_selection_atom = XInternAtom(display(), name, False);
        XSetSelectionOwner(display(), net_selection_atom, parent->winId(), CurrentTime);

        if (XGetSelectionOwner(display(), net_selection_atom) == parent->winId()) {
                XClientMessageEvent xev;

                xev.type = ClientMessage;
                xev.window = rootWindow();
                xev.message_type = Atoms::atom(Atoms::MANAGER);
                xev.format = 32;
                xev.data.l[0] = CurrentTime;
                xev.data.l[1] = net_selection_atom;
                xev.data.l[2] = parent->winId();
                xev.data.l[3] = 0; /* manager specific data */
                xev.data.l[4] = 0; /* manager specific data */

                XSendEvent(display(), parent->winId(), False, StructureNotifyMask, (XEvent *)&xev);
        }
}

Systray::~Systray()
{
    delete layout;
}


void Systray::addEmbed(Window wId)
{
        XResizeWindow(display(), wId, embedSize, embedSize);
        XMapWindow(display(), wId);

        container = new XContainer(this);
        container->setContentsMargins(0, 0, 0, 0);
        container->setFixedSize(embedSize, embedSize);

        container->embedClient(wId);

        embed.insert(wId, container); // save the Client winId/QX11EmbedContainer
        //qDebug() << "Client added to System Tray." << "QX11EmbedContainer id:" << container->winId() << "Client Id:" << wId;

        layout->addWidget(container);
}


void Systray::removeEmbed(Window wId)
{
        if (embed.contains(wId)) {
                container = embed.take(wId);
                //qDebug() << "QX11EmbedContainer:" << container->winId() << "Client:" << wId << "remove from Systray cmd.";
                layout->removeWidget(container);
                container->close();
                container->deleteLater();
        }
}
