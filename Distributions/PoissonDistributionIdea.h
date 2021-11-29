#ifndef POISSONDISTRIBUTIONIDEA_H
#define POISSONDISTRIBUTIONIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct PoissonDistributionAttributes : public IdeaAttributes
{
    PoissonDistributionAttributes();
};

class PoissonDistributionIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<bool> options READ options NOTIFY optionsChanged)

    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)

public:
    explicit PoissonDistributionIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

    QList<bool> options() const;

    double mean() const;

public slots:
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);
    void setOption(bool option, int index);

    void setMean(double mean);

signals:
    void optionsChanged(QList<bool> options);

    void meanChanged(double mean);

private:
    std::weak_ptr<NumberDataType> m_meanInput;
    std::shared_ptr<DistributionDataType> m_distribution;

    void validate();
    void compute(double mean);

    DistributionDisplay* m_display;

    QList<bool> m_options;

    double m_mean;
};

#endif // POISSONDISTRIBUTIONIDEA_H
