#ifndef MEANFROMDISTRIBUTIONSAMPLERIDEA_H
#define MEANFROMDISTRIBUTIONSAMPLERIDEA_H

#include <QPointF>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct MeanFromDistributionSamplerAttributes : public IdeaAttributes
{
    MeanFromDistributionSamplerAttributes();
};

class MeanFromDistributionSamplerIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<QVariant> samples READ samples WRITE setSamples NOTIFY samplesChanged)
    Q_PROPERTY(int totalSamples READ totalSamples WRITE setTotalSamples NOTIFY totalSamplesChanged)
    Q_PROPERTY(int samplingSize READ samplingSize WRITE setSamplingSize NOTIFY samplingSizeChanged)
    Q_PROPERTY(int samplesPerMean READ samplesPerMean WRITE setSamplesPerMean NOTIFY samplesPerMeanChanged)

    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)

public:
    explicit MeanFromDistributionSamplerIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void resetDisplay() override;

    QList<double> bounds() const;

    QList<QVariant> samples() const;
    int totalSamples() const;
    int samplingSize() const;
    int samplesPerMean() const;

    double animationSpeed() const;

signals:
    void samplesChanged(QList<QVariant> samples);
    void samplingSizeChanged(int samplingSize);

    void totalSamplesChanged(int totalSamples);  
    void samplesPerMeanChanged(int samplesPerMean);

    void startAnimation();
    void resetAnimation();
    void animationSpeedChanged(double animationSpeed);

public slots:
    void setBound(double bound, int index);
    void setAutoFit(bool autoFit);

    void setSamples(QList<QVariant> samples);
    void setTotalSamples(int totalSamples);
    void setSamplingSize(int samplingSize);
    void setSamplesPerMean(int samplesPerMean);

    void sampleButtonPressed();
    void resampleButtonPressed();
    void totalSamplesTextEdited(int samples);

    QPointF getPositionForSampleDot(int i);
    QList<double> getPositionsForMeanDots(int i);

    void resetSamples();

    void setAnimationSpeed(double animationSpeed);

private:
    std::weak_ptr<DistributionDataType> m_distribution;
    std::shared_ptr<NumbersDataType> m_samplesPointer;

    void validate();
    void plot();
    void sample();

    std::vector<double> m_xValues;
    std::vector<double> m_yValues;

    DistributionDisplay* m_display;

    std::vector<double> m_cdf;

    QList<QVariant> m_samplesForGridView;
    QList<int> m_samplesInImplicitBin;
    QList<double> m_sampleMeans;
    QList<QList<double>> m_samplesFromDistribution;

    int m_samplingSize;
    int m_currentSamples;
    int m_totalSamples;
    int m_samplesPerMean;

    double m_animationSpeed;
    DistributionDataType::DistributionType m_distributionType;
};

#endif // MEANFROMDISTRIBUTIONSAMPLERIDEA_H
