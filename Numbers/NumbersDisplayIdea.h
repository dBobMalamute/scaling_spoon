#ifndef NUMBERSDISPLAYIDEA_H
#define NUMBERSDISPLAYIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"

struct NumbersDisplayAttributes : public IdeaAttributes
{
    NumbersDisplayAttributes();
};

class NumbersDisplayIdea : public Idea
{
    Q_OBJECT

public:
    explicit NumbersDisplayIdea();

    const IdeaAttributes attributes() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;

signals:
    void displayNums(QString value);
    void clearNums();

public slots:

private:
    std::weak_ptr<NumbersDataType> m_nums;

};

#endif // NUMBERSDISPLAYIDEA_H
