#include <RectButton>
#include <GradientButton>
#include <HelpButton>
#include "ame_designer_plugin.h"

AmeWidgetInterface::AmeWidgetInterface(QObject *parent)
        : QObject(parent),
          initialized(false),
          d_isContainer(false)
{
}

bool AmeWidgetInterface::isContainer() const
{
        return d_isContainer;
}

bool AmeWidgetInterface::isInitialized() const
{
        return initialized;
}

QIcon AmeWidgetInterface::icon() const
{
        return d_icon;
}

QString AmeWidgetInterface::codeTemplate() const
{
        return d_codeTemplate;
}

QString AmeWidgetInterface::domXml() const
{
        return d_domXml;
}

QString AmeWidgetInterface::group() const
{
        return "Ame widgets";
}

QString AmeWidgetInterface::includeFile() const
{
        return d_include;
}

QString AmeWidgetInterface::name() const
{
        return d_name;
}

QString AmeWidgetInterface::toolTip() const
{
        return d_toolTip;
}

QString AmeWidgetInterface::whatsThis() const
{
        return d_whatsThis;
}

void AmeWidgetInterface::initialize(QDesignerFormEditorInterface *)
{
        if (initialized) return;
        initialized = true;
}

AmeRectButtonInterface::AmeRectButtonInterface(QObject *parent)
        : AmeWidgetInterface(parent)
{
        d_name = "AmeRectButton";
        d_include = "RectButton";
        d_icon = QIcon();
        d_domXml = "<ui language=\"c++\">\n"
                " <widget class=\"AmeRectButton\" name=\"rectButton\">\n"
                "  <property name=\"geometry\">\n"
                "   <rect>\n"
                "    <x>0</x>\n"
                "    <y>0</y>\n"
                "    <width>28</width>\n"
                "    <height>22</height>\n"
                "   </rect>\n"
                "  </property>\n"
                " </widget>\n"
                "</ui>\n";
}

QWidget *AmeRectButtonInterface::createWidget(QWidget *parent)
{
        return new AmeRectButton(parent);
}

AmeGradientButtonInterface::AmeGradientButtonInterface(QObject *parent)
        : AmeWidgetInterface(parent)
{
        d_name = "AmeGradientButton";
        d_include = "GradientButton";
        d_icon = QIcon();
        d_domXml = "<ui language=\"c++\">\n"
                " <widget class=\"AmeGradientButton\" name=\"gradientButton\">\n"
                "  <property name=\"geometry\">\n"
                "   <rect>\n"
                "    <x>0</x>\n"
                "    <y>0</y>\n"
                "    <width>22</width>\n"
                "    <height>20</height>\n"
                "   </rect>\n"
                "  </property>\n"
                " </widget>\n"
                "</ui>\n";
}

QWidget *AmeGradientButtonInterface::createWidget(QWidget *parent)
{
        return new AmeGradientButton(parent);
}

AmeHelpButtonInterface::AmeHelpButtonInterface(QObject *parent)
        : AmeWidgetInterface(parent)
{
        d_name = "AmeHelpButton";
        d_include = "HelpButton";
        d_icon = QIcon();
        d_domXml = "<ui language=\"c++\">\n"
                " <widget class=\"AmeHelpButton\" name=\"helpButton\">\n"
                "  <property name=\"geometry\">\n"
                "   <rect>\n"
                "    <x>0</x>\n"
                "    <y>0</y>\n"
                "    <width>23</width>\n"
                "    <height>23</height>\n"
                "   </rect>\n"
                "  </property>\n"
                " </widget>\n"
                "</ui>\n";
}

QWidget *AmeHelpButtonInterface::createWidget(QWidget *parent)
{
        return new AmeHelpButton(parent);
}

//
//
//

AmeWidgetsCollectionInterface::AmeWidgetsCollectionInterface(QObject *parent)
        : QObject(parent)
{
        d_plugins.append(new AmeRectButtonInterface(this));
        d_plugins.append(new AmeGradientButtonInterface(this));
        d_plugins.append(new AmeHelpButtonInterface(this));
}

QList <QDesignerCustomWidgetInterface *> AmeWidgetsCollectionInterface::customWidgets() const
{
        return d_plugins;
}

Q_EXPORT_PLUGIN2(amewidgets, AmeWidgetsCollectionInterface)
