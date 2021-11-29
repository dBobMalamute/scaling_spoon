#ifndef DISTRIBUTIONVARIANCEIDEA_H
#define DISTRIBUTIONVARIANCEIDEA_H

#include <QObject>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"
#include <QRectF>

struct DistributionVarianceAttributes : public IdeaAttributes
{
    DistributionVarianceAttributes();
};

class DistributionVarianceIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)
    Q_PROPERTY(double variance READ variance WRITE setVariance NOTIFY varianceChanged)

    Q_PROPERTY(int numDots READ numDots WRITE setNumDots NOTIFY numDotsChanged)
    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)

public:
    explicit DistributionVarianceIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void resetDisplay() override;

    double mean() const;
    double variance() const;
    int numDots() const;
    double animationSpeed() const;

signals:
    void meanChanged(double mean);
    void varianceChanged(double variance);
    void numDotsChanged(int numDots);
    void animationSpeedChanged(double animationSpeed);

    void startAnimation(bool continuous);

public slots:
    void setBound(double bound, int index);
    void setAutoFit(bool autoFit);
    void setMean(double mean);
    void setVariance(double variance);
    void setNumDots(int numDots);
    void setAnimationSpeed(double animationSpeed);

    void replayButtonClicked();

    QRectF getRectangleParams(int i);

private:
    std::weak_ptr<DistributionDataType> m_dist;
    std::shared_ptr<NumberDataType> m_output;

    void validate();
    void compute();
    void plot();
    void computeContinuousVisuals();
    void readyContinuousVisualsForQML();
    void readyDiscreteVisualsForQML();

    DistributionDisplay* m_display;

    std::vector<double> m_xValues;
    std::vector<double> m_yValues;
    DistributionDataType::DistributionType m_distributionType;

    std::vector<double> m_cdf;

    double m_mean;
    double m_variance;
    int m_numDots;
    double m_animationSpeed;

    //Continuous Visual
    QList<double> m_rectangleBounds;
    QList<double> m_rectangleHeights;
    QList<QRectF> m_rectangles;
};

#endif // DISTRIBUTIONVARIANCEIDEA_H
