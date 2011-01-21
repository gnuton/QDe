//////////////////////////////////////////
//  File      : systemicon.h		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#ifndef __SYSTEM_ICON_H
#define __SYSTEM_ICON_H

#include <QtGui>
#include <Global>

class AME_EXPORT AmeSystemIcon : public QIcon
{
public:
        enum IconType {
                None = 0,
                Custom,
                Action, Add, Remove, Ok, Cancel, Info, Refresh, Stop,
                Columns, CoverFlow, Icons, List,
                QuickLook, Find, Users,
                GoDown, GoHome, GoNext, GoPrevious, GoUp,
        };

        AmeSystemIcon(IconType type);
        ~AmeSystemIcon();
private:
        void createIcon(const QString &path);
};

#endif
