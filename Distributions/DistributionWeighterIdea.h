#ifndef DISTRIBUTIONWEIGHTERSIDEA_H
#define DISTRIBUTIONWEIGHTERSIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

struct DistributionWeighterAttributes : public IdeaAttributes
{
    DistributionWeighterAttributes();
};

class DistributionWeighterIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(double weight READ weight WRITE setWeight NOTIFY weightChanged)

public:
    explicit DistributionWeighterIdea();
    const IdeaAttributes attributes() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    double weight() const;
    Q_INVOKABLE QString topWeightString() const;
    Q_INVOKABLE QString botWeightString() const;

signals:
    void weightChanged();

public slots:

    void setWeight(double weight);

private:
    void validate();
    void computeContinuous();
    void computeDiscrete();

    std::shared_ptr<DistributionDataType> m_outputDist;
    std::weak_ptr<DistributionDataType> m_inputDist1;
    std::weak_ptr<DistributionDataType> m_inputDist2;

    double m_weight;
};

#endif // DISTRIBUTIONWEIGHTERSIDEA_H
