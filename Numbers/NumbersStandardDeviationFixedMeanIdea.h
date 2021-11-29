#ifndef NUMBERSSTANDARDDEVIATIONFIXEDMEANIDEA_H
#define NUMBERSSTANDARDDEVIATIONFIXEDMEANIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"
#include "../DataTypes/NumberDataType.h"

struct NumbersStandardDeviationFixedMeanAttributes : public IdeaAttributes
{
    NumbersStandardDeviationFixedMeanAttributes();
};

class NumbersStandardDeviationFixedMeanIdea : public Idea
{
    Q_OBJECT
    Q_PROPERTY(double lowBound READ lowBound WRITE setLowBound NOTIFY lowBoundChanged)
    Q_PROPERTY(double upBound READ upBound WRITE setUpBound NOTIFY upBoundChanged)
    Q_PROPERTY(bool autoFit READ autoFit WRITE setAutoFit NOTIFY autoFitChanged)

    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)
    Q_PROPERTY(double stdev READ stdev WRITE setStdev NOTIFY stdevChanged)

    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)

public:
    explicit NumbersStandardDeviationFixedMeanIdea();

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
    double stdev() const;

    double animationSpeed() const;

signals:
    void lowBoundChanged(double lowBound);
    void upBoundChanged(double upBound);
    void autoFitChanged(bool autoFit);

    void meanChanged(double mean);
    void stdevChanged(double stdev);
    void newDots(int numberDots);

    void animationSpeedChanged(double animationSpeed);

public slots:
    void setLowBound(double lowBound);
    void setUpBound(double upBound);
    void setAutoFit(bool autoFit);

    void replay();

    void setMean(double mean);
    void setStdev(double stdev);

    double getPositionForDot(int i);
    void setAnimationSpeed(double animationSpeed);


private:
    std::weak_ptr<NumbersDataType> m_inputNums;
    std::weak_ptr<NumberDataType> m_inputMean;
    std::shared_ptr<NumberDataType> m_outputStdev;

    void validate();
    void compute();

    double m_lowBound;
    double m_upBound;
    bool m_autoFit;

    double m_mean;
    double m_stdev;
    QList<double> m_nums;

    double m_animationSpeed;
};

#endif // NUMBERSSTANDARDDEVIATIONFIXEDMEANIDEA_H
