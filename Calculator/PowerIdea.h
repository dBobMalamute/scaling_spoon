#ifndef POWERIDEA_H
#define POWERIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct PowerAttributes : public IdeaAttributes
{
    PowerAttributes();
};

class PowerIdea : public Idea
{
    Q_OBJECT

public:
    PowerIdea();
    const IdeaAttributes attributes() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

signals:
    void displayNumbers(double input1, double input2, double output);
    void clearNumbers();

private:
    std::weak_ptr<NumberDataType> m_base;
    std::weak_ptr<NumberDataType> m_pow;

    std::shared_ptr<NumberDataType> m_result;

    void validate();
    void compute(double n1, double n2);
};

#endif // POWERIDEA_H
