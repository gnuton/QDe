#ifndef __GRADBUTTON_H
#define __GRADBUTTON_H

#include <QtGui>
#include <Global>
#include <SystemIcon>


class AME_EXPORT AmeGradientButton : public QToolButton
{
        Q_OBJECT

        Q_PROPERTY(ButtonCmd buttonCmd READ buttonCmd WRITE setButtonCmd)
        Q_ENUMS(ButtonCmd)

        Q_PROPERTY(GroupPosition groupPosition READ groupPosition WRITE setGroupPosition)
        Q_ENUMS(GroupPosition)
public:
        enum GroupPosition { LeftButton = 0, MidButton = 1, RightButton = 2 };
        enum ButtonCmd {
                None = 0,
                Custom,
                Action, Add, Remove, Ok, Cancel, Info, Refresh, Stop,
                Columns, CoverFlow, Icons, List,
                QuickLook, Find, Users,
                GoDown, GoHome, GoNext, GoPrevious, GoUp,
        };

        explicit AmeGradientButton(QWidget *parent = 0);
        AmeGradientButton(const ButtonCmd c, QWidget *parent = 0);
        ~AmeGradientButton();

        virtual QSize sizeHint() const;
        virtual QSize minimumSizeHint() const;

        void setGroupPosition(GroupPosition g);
        inline GroupPosition groupPosition() const { return gPos; }

        void setButtonCmd(ButtonCmd c);
        inline ButtonCmd buttonCmd() const { return cmd; }

protected:
        GroupPosition gPos;
        ButtonCmd cmd;
        virtual void paintEvent(QPaintEvent *event);

private:
        int w1;
        void restrictSize();

};

#endif
