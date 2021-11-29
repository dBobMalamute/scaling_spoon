#ifndef DISTRIBUTIONDISPLAYIDEA_H
#define DISTRIBUTIONDISPLAYIDEA_H

#include <QPointF>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct DistributionDisplayAttributes : public IdeaAttributes
{
    DistributionDisplayAttributes();
};

class DistributionDisplayIdea : public Idea
{
    Q_OBJECT

public:
    explicit DistributionDisplayIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

private:
    std::weak_ptr<DistributionDataType> m_distribution;

    void validate();
signals:

public slots:
    // xlow, xHigh, yLow, yHigh;
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);

private:
    DistributionDisplay* m_display;
};

#endif // DISTRIBUTIONDISPLAYIDEA_H
