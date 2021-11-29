#ifndef BETADISTRIBUTIONIDEA_H
#define BETADISTRIBUTIONIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct BetaDistributionAttributes : public IdeaAttributes
{
    BetaDistributionAttributes();
};

class BetaDistributionIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<bool> options READ options NOTIFY optionsChanged)

    Q_PROPERTY(double alpha READ alpha WRITE setAlpha NOTIFY alphaChanged)
    Q_PROPERTY(double beta READ beta WRITE setBeta NOTIFY betaChanged)

public:
    explicit BetaDistributionIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

    QList<bool> options() const;

    double alpha() const;
    double beta() const;

public slots:
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);
    void setOption(bool option, int index);

    void setAlpha(double alpha);
    void setBeta(double beta);

signals:
    void optionsChanged(QList<bool> options);

    void alphaChanged(double alpha);
    void betaChanged(double beta);

private:
    std::weak_ptr<NumberDataType> m_alphaInput;
    std::weak_ptr<NumberDataType> m_betaInput;

    std::shared_ptr<DistributionDataType> m_distribution;

    void validate();
    void compute(double alpha, double beta);

    DistributionDisplay* m_display;

    QList<bool> m_options;
    double m_alpha;
    double m_beta;
};

#endif // BETADISTRIBUTIONIDEA_H
