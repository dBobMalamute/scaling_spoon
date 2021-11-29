#ifndef POWERLAWDISTRIBUTIONIDEA_H
#define POWERLAWDISTRIBUTIONIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

struct PowerLawDistributionAttributes : public IdeaAttributes
{
    PowerLawDistributionAttributes();
};

class PowerLawDistributionIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<double> bounds READ bounds WRITE setBounds NOTIFY boundsChanged)
    Q_PROPERTY(QList<bool> options READ options WRITE setOptions NOTIFY optionsChanged)

    Q_PROPERTY(double decay READ decay WRITE setDecay NOTIFY decayChanged)
    Q_PROPERTY(double low READ low WRITE setLow NOTIFY lowChanged)

public:
    explicit PowerLawDistributionIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

    QList<double> bounds() const;
    QList<bool> options() const;

    double decay() const;
    double low() const;

public slots:
    void setBounds(QList<double> bounds);
    void setBound(double bound, int index);
    void setOptions(QList<bool> options);
    void setOption(bool option, int index);

    void fitBounds();

    void setDecay(double decay);
    void setLow(double low);

signals:
    void plot(std::vector<double> xValues, std::vector<double> yValues);

    void boundsChanged(QList<double> bounds);
    void optionsChanged(QList<bool> options);

    void decayChanged(double decay);
    void lowChanged(double low);

private:
    std::weak_ptr<NumberDataType> m_decayInput;
    std::weak_ptr<NumberDataType> m_lowInput;

    std::shared_ptr<DistributionDataType> m_distribution;

    void validate();
    void compute();

    std::vector<double> m_xValues;
    std::vector<double> m_yValues;

    QList<double> m_bounds;
    QList<bool> m_options;
    double m_decay;
    double m_low;
};

#endif // POWERLAWDISTRIBUTIONIDEA_H
