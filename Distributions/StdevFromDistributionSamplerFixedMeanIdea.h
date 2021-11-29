#ifndef STDEVFROMDISTRIBUTIONSAMPLERFIXEDMEANIDEA_H
#define STDEVFROMDISTRIBUTIONSAMPLERFIXEDMEANIDEA_H

#include <QPointF>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct StdevFromDistributionSamplerFixedMeanAttributes : public IdeaAttributes
{
    StdevFromDistributionSamplerFixedMeanAttributes();
};

class StdevFromDistributionSamplerFixedMeanIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)

    Q_PROPERTY(QList<QVariant> samples READ samples WRITE setSamples NOTIFY samplesChanged)
    Q_PROPERTY(int totalSamples READ totalSamples WRITE setTotalSamples NOTIFY totalSamplesChanged)
    Q_PROPERTY(int samplingSize READ samplingSize WRITE setSamplingSize NOTIFY samplingSizeChanged)
    Q_PROPERTY(int samplesPerStdev READ samplesPerStdev WRITE setSamplesPerStdev NOTIFY samplesPerStdevChanged)

    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)

public:
    explicit StdevFromDistributionSamplerFixedMeanIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void resetDisplay() override;

    double mean() const;

    QList<double> bounds() const;

    QList<QVariant> samples() const;
    int totalSamples() const;
    int samplingSize() const;
    int samplesPerStdev() const;

    double animationSpeed() const;   

signals:
    void meanChanged(double mean);

    void samplesChanged(QList<QVariant> samples);
    void samplingSizeChanged(int samplingSize);

    void totalSamplesChanged(int totalSamples);  
    void samplesPerStdevChanged(int samplesPerStdev);

    void startAnimation();
    void resetAnimation();
    void animationSpeedChanged(double animationSpeed);

public slots:
    void setMean(double mean);

    void setBound(double bound, int index);
    void setAutoFit(bool autoFit);

    void setSamples(QList<QVariant> samples);
    void setTotalSamples(int totalSamples);
    void setSamplingSize(int samplingSize);
    void setSamplesPerStdev(int samplesPerStdev);

    void sampleButtonPressed();
    void resampleButtonPressed();
    void totalSamplesTextEdited(int samples);

    QList<double> getPositionsForDots(int i);
    double getPositionForMeanLine();
    double getStdevLineLength(int i);

    void resetSamples();

    void setAnimationSpeed(double animationSpeed); 

private:
    std::weak_ptr<DistributionDataType> m_distribution;
    std::weak_ptr<NumberDataType> m_inputMean;
    std::shared_ptr<NumbersDataType> m_samplesPointer;

    void validate();
    void plot();
    void sample();

    double m_mean;

    std::vector<double> m_xValues;
    std::vector<double> m_yValues;

    DistributionDisplay* m_display;

    std::vector<double> m_cdf;

    QList<QVariant> m_samplesForGridView;
    QList<double> m_sampleStdevs;
    QList<QList<double>> m_samplesFromDistribution;

    int m_samplingSize;
    int m_currentSamples;
    int m_totalSamples;
    int m_samplesPerStdev;

    double m_animationSpeed;
    DistributionDataType::DistributionType m_distributionType;
};

#endif // STDEVFROMDISTRIBUTIONSAMPLERFIXEDMEANIDEA_H
