#ifndef __DESIGNER_PLUGIN_H
#define __DESIGNER_PLUGIN_H

#include <QDesignerCustomWidgetInterface>

class AmeWidgetInterface : public QObject, public QDesignerCustomWidgetInterface
{
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
        AmeWidgetInterface(QObject *parent = 0);

        virtual bool isContainer() const;
        virtual bool isInitialized() const;
        virtual QIcon icon() const;
        virtual QString codeTemplate() const;
        virtual QString domXml() const;
        virtual QString group() const;
        virtual QString includeFile() const;
        virtual QString name() const;
        virtual QString toolTip() const;
        virtual QString whatsThis() const;
        virtual void initialize(QDesignerFormEditorInterface *);

private:
        bool initialized;

protected:
        bool d_isContainer;
        QString d_name;
        QString d_include;
        QString d_toolTip;
        QString d_whatsThis;
        QString d_domXml;
        QString d_codeTemplate;
        QIcon d_icon;
};

class AmeWidgetsCollectionInterface : public QObject, public QDesignerCustomWidgetCollectionInterface
{
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
        AmeWidgetsCollectionInterface(QObject *parent = 0);

        virtual QList<QDesignerCustomWidgetInterface *> customWidgets() const;

private:
        QList<QDesignerCustomWidgetInterface *> d_plugins;
};


class AmeRectButtonInterface : public AmeWidgetInterface
{
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
        AmeRectButtonInterface(QObject *parent);
        virtual QWidget *createWidget(QWidget *parent);

};

class AmeGradientButtonInterface : public AmeWidgetInterface
{
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
        AmeGradientButtonInterface(QObject *parent);
        virtual QWidget *createWidget(QWidget *parent);

};

class AmeHelpButtonInterface : public AmeWidgetInterface
{
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
        AmeHelpButtonInterface(QObject *parent);
        virtual QWidget *createWidget(QWidget *parent);

};

#endif
