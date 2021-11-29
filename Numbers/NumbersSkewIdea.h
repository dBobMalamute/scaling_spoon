#ifndef NUMBERSSKEWIDEA_H
#define NUMBERSSKEWIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"
#include "../DataTypes/NumberDataType.h"

struct NumbersSkewIdeaAttributes : public IdeaAttributes
{
    NumbersSkewIdeaAttributes();
};

class NumbersSkewIdea : public Idea
{
    Q_OBJECT
    Q_PROPERTY(double lowBound READ lowBound WRITE setLowBound NOTIFY lowBoundChanged)
    Q_PROPERTY(double upBound READ upBound WRITE setUpBound NOTIFY upBoundChanged)
    Q_PROPERTY(bool autoFit READ autoFit WRITE setAutoFit NOTIFY autoFitChanged)

    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)
    Q_PROPERTY(double skew READ skew WRITE setSkew NOTIFY skewChanged)

    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)

public:
    explicit NumbersSkewIdea();

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
    double skew() const;

    double animationSpeed() const;


signals:
    void lowBoundChanged(double lowBound);
    void upBoundChanged(double upBound);
    void autoFitChanged(bool autoFit);

    void meanChanged(double mean);
    void skewChanged(double skew);

    void newDots(int numberDots);

    void animationSpeedChanged(double animationSpeed);


public slots:
    void setLowBound(double lowBound);
    void setUpBound(double upBound);
    void setAutoFit(bool autoFit);

    void replay();
    void setMean(double mean);
    void setSkew(double skew);

    double getPositionForDot(int i);
    void setAnimationSpeed(double animationSpeed);


private:
    std::weak_ptr<NumbersDataType> m_inputNums;
    std::shared_ptr<NumberDataType> m_outputSkew;

    void validate();
    void compute();

    double m_lowBound;
    double m_upBound;
    bool m_autoFit;

    double m_mean;
    double m_skew;

    QList<double> m_nums;

    double m_animationSpeed;
};

#endif // NUMBERSSKEWIDEA_H
