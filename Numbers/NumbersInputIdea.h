#ifndef NUMBERSINPUTIDEA_H
#define NUMBERSINPUTIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"

struct NumbersInputAttributes : public IdeaAttributes
{
    NumbersInputAttributes();
};

class NumbersInputIdea : public Idea
{
    Q_OBJECT

public:
    explicit NumbersInputIdea();

    const IdeaAttributes attributes() override;

    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

signals:
    void numsChanged();

public slots:
    void setNums(QString nums);
    QString getNums();

private:
    std::shared_ptr<NumbersDataType> m_nums;
};

#endif // NUMBERSINPUTIDEA_H
