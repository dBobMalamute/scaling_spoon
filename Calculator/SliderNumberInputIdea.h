#ifndef SLIDERNUMBERINPUTIDEA_H
#define SLIDERNUMBERINPUTIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct SliderNumberInputAttributes : public IdeaAttributes
{
    SliderNumberInputAttributes();
};

class SliderNumberInputIdea: public Idea
{
    Q_OBJECT

public:
    SliderNumberInputIdea();
    const IdeaAttributes attributes() override;

    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(QJsonObject const &p) override;

    Q_INVOKABLE double number();
    Q_INVOKABLE double upLimit();
    Q_INVOKABLE double lowLimit();
    Q_INVOKABLE double sliderPos() const;

signals:
    void numberChanged();
    void upLimitChanged();
    void lowLimitChanged();

public slots:
    void setNumber(double number);
    void setUpLimit(double upLimit);
    void setLowLimit(double lowLimit);
    void setNumberBySlider(double value);

    void updateOutputNumber();


private:
    std::shared_ptr<NumberDataType> m_output;

    double dealWithFloatingPointJunk(double input);

    double m_number;
    double m_upLimit;
    double m_lowLimit;

};

#endif //INPUTNUMBERSLIDERIDEA_H





