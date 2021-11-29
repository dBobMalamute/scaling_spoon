#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <QObject>
#include <QtPlugin>
#include <QDebug>
#include <QString>
#include "../MalamuteCore/InheritedClasses/MalamutePlugin.h"

class DistributionsPlugin : public MalamutePlugin
{
    Q_OBJECT

public:
    QString PluginName() override;
    QStringList authors() override;
    QString PluginDescription() override;
    void registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry) override;
    void registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry) override;
    void registerToolbars(std::shared_ptr<ToolbarRegistry> toolbarRegistry) override;
};

#endif // DISTRIBUTIONS_H
