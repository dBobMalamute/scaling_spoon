#ifndef SQUAREDIDEA_H
#define SQUAREDIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct SquaredAttributes : public IdeaAttributes
{
    SquaredAttributes();
};

class SquaredIdea : public Idea
{
    Q_OBJECT

public:
    explicit SquaredIdea();
    const IdeaAttributes attributes() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

signals:
    void displayNumbers(double input, double output);
    void clearNumbers();

private:
    std::weak_ptr<NumberDataType> m_input;
    std::shared_ptr<NumberDataType> m_result;

    void validate();
    void compute(double n1);
};

#endif // SQUAREDIDEA_H
