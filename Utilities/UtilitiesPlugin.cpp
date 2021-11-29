#include "UtilitiesPlugin.h"

#include "TextAreaIdea.h"
#include "TextLineIdea.h"
#include "ColoredSquareIdea.h"

QString UtilitiesPlugin::PluginName()
{
    return "Utilites";
}

QStringList UtilitiesPlugin::authors()
{
    return QStringList("dBob");
}

QString UtilitiesPlugin::PluginDescription()
{
    return "This plugin holds some basic utilities for labeling a malamute diagram";
}

void UtilitiesPlugin::registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry)
{
    ideaRegistry->registerIdea<TextAreaIdea>();
    ideaRegistry->registerIdea<TextLineIdea>();
    //ideaRegistry->registerIdea<ColoredSquareIdea>();
}

void UtilitiesPlugin::registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry)
{
    Q_UNUSED(dataTypeRegistry)
}

void UtilitiesPlugin::registerToolbars(std::shared_ptr<ToolbarRegistry> toolbarRegistry)
{
    QStringList orderList;
    orderList << Magic<TextLineAttributes>::cast().listName();
    orderList << Magic<TextAreaAttributes>::cast().listName();
    //orderList << Magic<ColoredSquareAttributes>::cast().listName();

    toolbarRegistry->registerToolbar(PluginName(),orderList);
}
