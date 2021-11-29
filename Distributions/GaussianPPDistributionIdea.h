#ifndef GAUSSIANPPDISTRIBUTIONIDEA_H
#define GAUSSIANPPDISTRIBUTIONIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct GaussianPPDistributionAttributes : public IdeaAttributes
{
    GaussianPPDistributionAttributes();
};

class GaussianPPDistributionIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<bool> options READ options NOTIFY optionsChanged)

    Q_PROPERTY(QList<double> params READ params WRITE setParams NOTIFY paramsChanged)
    Q_PROPERTY(QList<bool> paramValid READ paramValid WRITE setParamValid NOTIFY paramValidChanged)

public:
    explicit GaussianPPDistributionIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

    QList<bool> options() const;

    QList<double> params() const;
    QList<bool> paramValid() const;

public slots:
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);
    void setOption(bool option, int index);
    void setParams(QList<double> params);
    void setParamValid(QList<bool> paramValid);

signals:
    void optionsChanged(QList<bool> options);
    void paramsChanged(QList<double> params);
    void paramValidChanged(QList<bool> paramValid);

private:
    std::weak_ptr<NumberDataType> m_lowInput;
    std::weak_ptr<NumberDataType> m_upInput;
    std::weak_ptr<NumberDataType> m_meanInput;
    std::weak_ptr<NumberDataType> m_stdevInput;
    std::weak_ptr<NumberDataType> m_skewInput;
    std::weak_ptr<NumberDataType> m_kurtosisInput;

    std::shared_ptr<DistributionDataType> m_distribution;

    void validate();

    bool validateLowUp(double low, double up);
    bool validateLowMean(double low, double mean);
    bool validateUpMean(double up, double mean);
    bool validateStdev(double stdev);
    bool validateStdevLowUp(double low, double up, double stdev);

    bool validateLowMeanStdev(double low, double mean, double stdev);
    bool validateUpMeanStdev(double up, double mean, double stdev);

    bool validateSkewKurtosis(double skew, double kurtosis);

    void computeSixParameter(double low, double up, double mean, double stdev, double skew, double kurtosis);
    void computeFourParameter(double low, double up, double mean, double stdev);
    void computeLowUpMean(double low, double up, double mean);
    void computeGaussian(double mean, double stdev);
    void computePositiveExponential(double low, double mean);
    void computeNegativeExponential(double up, double mean);
    void computeUniform(double low, double up);

    DistributionDisplay* m_display;

    QList<bool> m_options;

    QList<double> m_params;
    QList<bool> m_paramValid;
};

#endif // GAUSSIANPLUSPLUSDISTRIBUTIONIDEA_H
