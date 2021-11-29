#ifndef DISTRIBUTIONSAMPLERIDEA_H
#define DISTRIBUTIONSAMPLERIDEA_H

#include <QPointF>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct DistributionSamplerAttributes : public IdeaAttributes
{
    DistributionSamplerAttributes();
};

class DistributionSamplerIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<QVariant> samples READ samples WRITE setSamples NOTIFY samplesChanged)
    Q_PROPERTY(int totalSamples READ totalSamples WRITE setTotalSamples NOTIFY totalSamplesChanged)
    Q_PROPERTY(int samplingSize READ samplingSize WRITE setSamplingSize NOTIFY samplingSizeChanged)

public:
    explicit DistributionSamplerIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void resetDisplay() override;

    QList<QVariant> samples() const;
    int totalSamples() const;
    int samplingSize() const;

signals:
    void samplesChanged(QList<QVariant> samples);
    void samplingSizeChanged(int samplingSize);
    void totalSamplesChanged(int totalSamples);

    void startAnimation();
    void resetAnimation();

public slots:
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);

    void setSamples(QList<QVariant> samples);
    void setTotalSamples(int totalSamples);
    void setSamplingSize(int samplingSize);

    void sampleButtonPressed();
    void resampleButtonPressed();
    void totalSamplesTextEdited(int samples);

    QPointF getPositionForDot(int i);
    void resetSamples();

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

    QList<QVariant> m_samples;
    QList<int> m_samplesInImplicitBin;
    QList<double> m_samplesDoubles;
    int m_samplingSize;
    int m_currentSamples;
    int m_totalSamples;

    DistributionDataType::DistributionType m_distributionType;
};

#endif // DISTRIBUTIONDISPLAYIDEA_H
