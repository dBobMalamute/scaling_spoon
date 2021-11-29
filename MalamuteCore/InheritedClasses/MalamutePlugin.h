#ifndef MALAMUTEPLUGIN_H
#define MALAMUTEPLUGIN_H

#include <QString>
#include "../Registries/DataTypeRegistry.h"
#include "../Registries/IdeaRegistry.h"
#include "../Registries/ToolbarRegistry.h"

class MalamutePlugin : public QObject
{
public:
    virtual ~MalamutePlugin() = 0;

    virtual QString PluginName() = 0;
    virtual QString PluginDescription() = 0;
    virtual QStringList authors() = 0;

    virtual void registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry) = 0;
    virtual void registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry) = 0;
    virtual void registerToolbars(std::shared_ptr<ToolbarRegistry> toolbarRegistry) = 0;
};

#endif // MALAMUTEPLUGIN_H
