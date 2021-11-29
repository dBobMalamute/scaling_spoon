#ifndef BINOMIALDISTRIBUTIONIDEA_H
#define BINOMIALDISTRIBUTIONIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct BinomialDistributionAttributes : public IdeaAttributes
{
    BinomialDistributionAttributes();
};

class BinomialDistributionIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<bool> options READ options NOTIFY optionsChanged)

public:
    explicit BinomialDistributionIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

    QList<bool> options() const;

public slots:
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);
    void setOption(bool option, int index);

signals:
    void optionsChanged(QList<bool> options);

private:
    std::weak_ptr<NumberDataType> m_numberInput;
    std::weak_ptr<NumberDataType> m_probInput;

    std::shared_ptr<DistributionDataType> m_distribution;

    void validate();
    void compute(int number, double prob);

    DistributionDisplay* m_display;

    QList<bool> m_options;
};

#endif // BINOMIALDISTRIBUTIONIDEA_H
