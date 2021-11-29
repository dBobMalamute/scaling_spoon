#ifndef POWEROFTWOIDEA_H
#define POWEROFTWOIDEA_H

#include <QObject>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct PowerOfTwoIdeaAttributes : public IdeaAttributes
{
    PowerOfTwoIdeaAttributes();
};

class PowerOfTwoIdea : public Idea
{
    Q_OBJECT

public:
    explicit PowerOfTwoIdea();
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
    void compute(double n1);
};

#endif // POWEROFTWOIDEA_H
