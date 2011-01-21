//////////////////////////////////////////
//  File      : systemicon.cpp		//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#include "systemicon.h"

AmeSystemIcon::AmeSystemIcon(IconType type)
        : QIcon()
{
        switch (type) {
        case Action: {
                        createIcon(":/sysicons/action%1.png");
                        break;
                }
        case Add: {
                        createIcon(":/sysicons/add%1.png");
                        break;
                }
        case Remove: {
                        createIcon(":/sysicons/remove%1.png");
                        break;
                }
        case Ok: {
                        createIcon(":/sysicons/ok%1.png");
                        break;
                }
        case Cancel: {
                        createIcon(":/sysicons/cancel%1.png");
                        break;
                }
        case Info: {
                        createIcon(":/sysicons/info%1.png");
                        break;
                }
        case Refresh: {
                        createIcon(":/sysicons/refresh%1.png");
                        break;
                }


        case Columns: {
                        createIcon(":/sysicons/columns%1.png");
                        break;
                }
        case Icons: {
                        createIcon(":/sysicons/icons%1.png");
                        break;
                }
        case List: {
                        createIcon(":/sysicons/list%1.png");
                        break;
                }



        case GoPrevious: {
                        createIcon(":/sysicons/go-prev%1.png");
                        break;
                }
        case GoNext: {
                        createIcon(":/sysicons/go-next%1.png");
                        break;
                }


        }
}

AmeSystemIcon::~AmeSystemIcon()
{
}

void AmeSystemIcon::createIcon(const QString &path)
{
        addFile(path.arg(0), QSize(16,16), Normal, On);
        addFile(path.arg(0), QSize(16,16), Normal, Off);

        addFile(path.arg(1), QSize(16,16), Selected, On);
        addFile(path.arg(1), QSize(16,16), Selected, Off);
}
