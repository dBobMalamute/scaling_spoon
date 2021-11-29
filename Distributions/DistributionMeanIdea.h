#ifndef DISTRIBUTIONMEANIDEA_H
#define DISTRIBUTIONMEANIDEA_H

#include <QObject>
#include <QPointF>
#include <QRectF>

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct DistributionMeanAttributes : public IdeaAttributes
{
    DistributionMeanAttributes();
};

class DistributionMeanIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)

    Q_PROPERTY(int numDots READ numDots WRITE setNumDots NOTIFY numDotsChanged)
    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)

public:
    explicit DistributionMeanIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void resetDisplay() override;

    double mean() const;
    int numDots() const;
    double animationSpeed() const;

signals:
    void meanChanged(double mean);
    void numDotsChanged(int numDots);
    void animationSpeedChanged(double animationSpeed);

    void startAnimation(bool continuous);

public slots:
    void setBound(double bound, int index);
    void setAutoFit(bool autoFit);
    void setMean(double mean);
    void setNumDots(int numDots);
    void setAnimationSpeed(double animationSpeed);

    void replayButtonClicked();

    QRectF getRectangleParams(int i);
    QPointF getPointParams(int i);
    int getNumDiscretePoints();

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

    QList<double> m_rectangleBounds;
    QList<double> m_rectangleHeights;

    QList<QRectF> m_rectangles;
    QList<QPointF> m_dotPositions;

    std::vector<double> m_cdf;
    DistributionDataType::DistributionType m_distributionType;
    double m_mean;
    int m_numDots;
    double m_animationSpeed;
};

#endif // MEANIDEA_H
