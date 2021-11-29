#ifndef UNIFORMDISTRIBUTIONIDEA_H
#define UNIFORMDISTRIBUTIONIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct UniformDistributionAttributes : public IdeaAttributes
{
    UniformDistributionAttributes();
};

class UniformDistributionIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<bool> options READ options NOTIFY optionsChanged)

    Q_PROPERTY(double low READ low WRITE setLow NOTIFY lowChanged)
    Q_PROPERTY(double up READ up WRITE setUp NOTIFY upChanged)

public:
    explicit UniformDistributionIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

    QList<bool> options() const;

    double low() const;
    double up() const;

public slots:
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);
    void setOption(bool option, int index);

    void setLow(double low);
    void setUp(double up);

signals:
    void optionsChanged(QList<bool> options);

    void lowChanged(double low);
    void upChanged(double up);

private:
    std::weak_ptr<NumberDataType> m_lowInput;
    std::weak_ptr<NumberDataType> m_upInput;

    std::shared_ptr<DistributionDataType> m_distribution;

    void validate();
    void compute(double low, double up);

    DistributionDisplay* m_display;

    QList<bool> m_options;
    double m_low;
    double m_up;
};

#endif // UNIFORMDISTRIBUTIONIDEA_H
