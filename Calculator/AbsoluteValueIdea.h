#ifndef ABSOLUTEVALUEIDEA_H
#define ABSOLUTEVALUEIDEA_H

#include <QObject>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct AbsoluteValueAttributes : public IdeaAttributes
{
    AbsoluteValueAttributes();
};

class AbsoluteValueIdea : public Idea
{
    Q_OBJECT

public:
    AbsoluteValueIdea();
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
    void compute(double n);
};





#endif // ABSOLUTEVALUEIDEA_H
