#ifndef ADDITIONIDEA_H
#define ADDITIONIDEA_H

#include <QObject>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct AdditionAttributes : public IdeaAttributes
{
    AdditionAttributes();
};

class AdditionIdea : public Idea
{
    Q_OBJECT

public:
    AdditionIdea();
    const IdeaAttributes attributes() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

signals:
    void displayNumbers(double input1, double input2, double output);
    void clearNumbers();

private:
    std::weak_ptr<NumberDataType> m_number1;
    std::weak_ptr<NumberDataType> m_number2;

    std::shared_ptr<NumberDataType> m_result;

    void validate();
    void compute(double n1, double n2);
};

#endif // ADDITIONIDEA_H
