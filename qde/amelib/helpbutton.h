#ifndef __HELPBUTTON_H
#define __HELPBUTTON_H

#include <QtGui>
#include <Global>

class AME_EXPORT AmeHelpButton : public QToolButton
{
        Q_OBJECT

public:
        explicit AmeHelpButton(QWidget * parent = 0);
        AmeHelpButton(const QString &url, QWidget *parent = 0);
        ~AmeHelpButton();

        virtual QSize sizeHint() const;
        virtual QSize minimumSizeHint() const;

protected:
        QString helpUrl;

        virtual void paintEvent(QPaintEvent *event);

private:
        void restrictSize();

};

#endif
