#ifndef GAUSSIANDISTRIBUTIONIDEA_H
#define GAUSSIANDISTRIBUTIONIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct GaussianDistributionAttributes : public IdeaAttributes
{
    GaussianDistributionAttributes();
};

class GaussianDistributionIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<bool> options READ options NOTIFY optionsChanged)

    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)
    Q_PROPERTY(double stdev READ stdev WRITE setStdev NOTIFY stdevChanged)

public:
    explicit GaussianDistributionIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

    QList<bool> options() const;

    double mean() const;
    double stdev() const;

public slots:
// xlow, xHigh, yLow, yHigh;
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);
    void setOption(bool option, int index);

    void setMean(double mean);
    void setStdev(double stdev);

signals:
    void optionsChanged(QList<bool> options);

    void meanChanged(double mean);
    void stdevChanged(double stdev);

private:
    std::weak_ptr<NumberDataType> m_meanInput;
    std::weak_ptr<NumberDataType> m_stdevInput;

    std::shared_ptr<DistributionDataType> m_distribution;

    void validate();
    void compute(double mean, double stdev);

    DistributionDisplay* m_display;

    QList<bool> m_options;
    double m_mean;
    double m_stdev;
};

#endif // GAUSSIANIDEA_H
