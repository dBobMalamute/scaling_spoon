#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QtPlugin>
#include <QDebug>
#include <QString>
#include "../MalamuteCore/InheritedClasses/MalamutePlugin.h"

class CalculatorPlugin : public MalamutePlugin
{
    Q_OBJECT

public:
    QString PluginName() override;
    QString PluginDescription() override;
    QStringList authors() override;
    void registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry) override;
    void registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry) override;
    void registerToolbars(std::shared_ptr<ToolbarRegistry> toolbarRegistry) override;
};


#endif // CALCULATOR_H
