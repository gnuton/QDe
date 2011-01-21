//////////////////////////////////////////
//  File      : toolbutton.h		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#ifndef __RECTBUTTON_H
#define __RECTBUTTON_H

#include <Global>
#include <QtGui>
#include <SystemIcon>

class AME_EXPORT AmeRectButton : public QToolButton
{
        Q_OBJECT

        Q_PROPERTY(ButtonCmd buttonCmd READ buttonCmd WRITE setButtonCmd)
        Q_ENUMS(ButtonCmd)

        Q_PROPERTY(WSize widgetSize READ widgetSize WRITE setWidgetSize)
        Q_ENUMS(WSize)

public:
        enum WSize { MiniSize = 8, SmallSize = 10, NormalSize = 12 };
        enum ButtonStyle {IconButton, TextButton};
        enum ButtonCmd {
                None = 0,
                Custom,
                Action, Add, Remove, Ok, Cancel, Info, Refresh, Stop,
                Columns, CoverFlow, Icons, List,
                QuickLook, Find, Users,
                GoDown, GoHome, GoNext, GoPrevious, GoUp,
        };

        explicit AmeRectButton(QWidget *parent = 0);
        AmeRectButton(const QString &text, const QString &tooltip, QWidget *parent = 0);
        AmeRectButton(const ButtonCmd c, const QString &tooltip, QWidget *paerent = 0);
        AmeRectButton(const AmeSystemIcon &icon, const QString &tooltip, QWidget *parent = 0);
        ~AmeRectButton();

        virtual QSize sizeHint() const;
        virtual QSize minimumSizeHint() const;

        void setWidgetSize(WSize s);
        inline WSize widgetSize() const { return wsize; }

        void setButtonCmd(ButtonCmd c);
        inline ButtonCmd buttonCmd() const { return cmd; }

protected:
        ButtonStyle buttonStyle;
        WSize wsize;
        ButtonCmd cmd;

};


#endif
