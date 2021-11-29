#ifndef NUMBERWEIGHTERIDEA_H
#define NUMBERWEIGHTERIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct NumberWeighterAttributes : public IdeaAttributes
{
    NumberWeighterAttributes();
};

class NumberWeighterIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(double weight READ weight WRITE setWeight NOTIFY weightChanged)

public:
    NumberWeighterIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;


    double weight() const;

public slots:

    void setWeight(double weight);
    Q_INVOKABLE QString topWeightString() const;
    Q_INVOKABLE QString botWeightString() const;

signals:
    void weightChanged();

private:
    std::weak_ptr<NumberDataType> m_number1;
    std::weak_ptr<NumberDataType> m_number2;

    std::shared_ptr<NumberDataType> m_result;

    void validate();
    void compute(double weight, double n1, double n2);

    double m_weight;
};

#endif // NUMBERWEIGHTERIDEA_H
