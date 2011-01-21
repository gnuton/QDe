//////////////////////////////////////////
//  File      : icon.h			//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#ifndef __AMEICON_H
#define __AMEICON_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QMouseEvent>
#include <Global>
#include <IconTheme>

class AME_EXPORT AmeIcon : public QWidget
{
        Q_OBJECT

public:
        enum ScaleFactor {
                undefined = 0,
                verysmall = 22,
                small = 32,
                average = 40,
                medium = 48,
                normal = 64,
                large = 128,
                extralarge = 254,
                dynamic = 255 // or > 255
        };

        enum IconState {
                Normal,
                Disabled,
                Selected,
                Prelight
        };

        AmeIcon(QWidget *parent = 0);
        explicit AmeIcon(const QString &name, const QString &label,
                        int scale = normal, AmeIconTheme *theme=0, QWidget *parent = 0);
        explicit AmeIcon(const QString &label, const QPixmap &copy, int scale = normal, QWidget *parent = 0);

        void drawBorder(bool);
        void scale(int factor);
        virtual QSize sizeHint () const;
        int width();
        int height();

        virtual ~AmeIcon();

public slots:
        void setLabel(const QString &label);

signals:
        void clicked();

protected:
        void paintIcon();
        void paintLabel();
        void paintEvent (QPaintEvent *event);
        void mousePressEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);

private:
        class IconPrivate;
        IconPrivate * d;

        void squeezeText();
        void adjustFont();
};

#endif

