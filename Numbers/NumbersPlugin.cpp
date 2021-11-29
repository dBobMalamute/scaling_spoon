#include "NumbersPlugin.h"

#include "NumbersInputIdea.h"
#include "NumbersDisplayIdea.h"
#include "NumbersMeanIdea.h"
#include "NumbersBinnerIdea.h"
#include "NumbersStandardDeviationIdea.h"
#include "NumbersSkewIdea.h"
#include "NumbersVarianceIdea.h"
#include "NumbersStandardDeviationFixedMeanIdea.h"
#include "NumbersVarianceFixedMeanIdea.h"

#include "../DataTypes/NumbersDataType.h"
#include "../DataTypes/NumberDataType.h"

QString NumbersPlugin::PluginName()
{
    return "Numbers";
}

QStringList NumbersPlugin::authors()
{
    return QStringList("dBob");
}

QString NumbersPlugin::PluginDescription()
{
    return "This plugin holds the ideas usually used when analyzing numbers";
}

void NumbersPlugin::registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry)
{
    ideaRegistry->registerIdea<NumbersInputIdea>();
    ideaRegistry->registerIdea<NumbersDisplayIdea>();
    ideaRegistry->registerIdea<NumbersMeanIdea>();
    ideaRegistry->registerIdea<NumbersBinnerIdea>();
    ideaRegistry->registerIdea<NumbersStandardDeviationIdea>();
    ideaRegistry->registerIdea<NumbersVarianceIdea>();
    ideaRegistry->registerIdea<NumbersSkewIdea>();
    ideaRegistry->registerIdea<NumbersStandardDeviationFixedMeanIdea>();
    ideaRegistry->registerIdea<NumbersVarianceFixedMeanIdea>();
}

void NumbersPlugin::registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry)
{
    dataTypeRegistry->registerDataTypeAttributes<NumberDataTypeAttributes>();
    dataTypeRegistry->registerDataTypeAttributes<NumbersDataTypeAttributes>();
}

void NumbersPlugin::registerToolbars(std::shared_ptr<ToolbarRegistry> toolbarRegistry)
{
    QStringList orderList;
    orderList << Magic<NumbersInputAttributes>::cast().listName();
    orderList << Magic<NumbersDisplayAttributes>::cast().listName();
    orderList << Magic<NumbersMeanAttributes>::cast().listName();
    orderList << Magic<NumbersStandardDeviationAttributes>::cast().listName();
    orderList << Magic<NumbersVarianceAttributes>::cast().listName();
    orderList << Magic<NumbersSkewIdeaAttributes>::cast().listName();
    orderList << Magic<NumbersBinnerAttributes>::cast().listName();
    orderList << Magic<NumbersStandardDeviationFixedMeanAttributes>::cast().listName();
    orderList << Magic<NumbersVarianceFixedMeanAttributes>::cast().listName();

    toolbarRegistry->registerToolbar("List of Numbers", orderList);
}
