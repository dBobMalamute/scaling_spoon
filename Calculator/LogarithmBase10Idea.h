#ifndef LOGARITHMBASE10IDEA_H
#define LOGARITHMBASE10IDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct LogarithmBase10Attributes : public IdeaAttributes
{
    LogarithmBase10Attributes();
};

class LogarithmBase10Idea : public Idea
{
    Q_OBJECT
public:
    explicit LogarithmBase10Idea();
    const IdeaAttributes attributes() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    void resetDisplay() override;

signals:
    void displayNumbers(double input, double output);
    void clearNumbers();

private:
    std::weak_ptr<NumberDataType> m_input;
    std::shared_ptr<NumberDataType> m_result;

    void validate();
    void compute(double n);
};

#endif // LOGARITHMBASE10IDEA_H
