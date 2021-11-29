#ifndef NUMBERSVARIANCEFIXEDMEANIDEA_H
#define NUMBERSVARIANCEFIXEDMEANIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"
#include "../DataTypes/NumberDataType.h"

struct NumbersVarianceFixedMeanAttributes : public IdeaAttributes
{
    NumbersVarianceFixedMeanAttributes();
};

class NumbersVarianceFixedMeanIdea : public Idea
{
    Q_OBJECT
    Q_PROPERTY(double lowBound READ lowBound WRITE setLowBound NOTIFY lowBoundChanged)
    Q_PROPERTY(double upBound READ upBound WRITE setUpBound NOTIFY upBoundChanged)
    Q_PROPERTY(bool autoFit READ autoFit WRITE setAutoFit NOTIFY autoFitChanged)

    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)
    Q_PROPERTY(double variance READ variance WRITE setVariance NOTIFY varianceChanged)

    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)

public:
    explicit NumbersVarianceFixedMeanIdea();

    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

    double lowBound() const;
    double upBound() const; 
    bool autoFit() const;

    double mean() const;
    double variance() const;

    double animationSpeed() const;

signals:
    void lowBoundChanged(double lowBound);
    void upBoundChanged(double upBound);
    void autoFitChanged(bool autoFit);

    void meanChanged(double mean);
    void varianceChanged(double variance);
    void newDots(int numberDots);

    void animationSpeedChanged(double animationSpeed);

public slots:
    void setLowBound(double lowBound);
    void setUpBound(double upBound);
    void setAutoFit(bool autoFit);

    void replay();

    void setMean(double mean);
    void setVariance(double variance);

    double getPositionForDot(int i);
    void setAnimationSpeed(double animationSpeed);


private:
    std::weak_ptr<NumbersDataType> m_inputNums;
    std::weak_ptr<NumberDataType> m_inputMean;
    std::shared_ptr<NumberDataType> m_outputVariance;

    void validate();
    void compute();
    void computeWithPresetMean();

    double m_lowBound;
    double m_upBound;
    bool m_autoFit;

    double m_mean;
    double m_variance;
    QList<double> m_nums;

    double m_animationSpeed;
};

#endif // NUMBERSVARIANCEFIXEDMEANIDEA_H
