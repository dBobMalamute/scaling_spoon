#ifndef NUMBERINPUTIDEA_H
#define NUMBERINPUTIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct NumberInputAttributes : public IdeaAttributes
{
    NumberInputAttributes();
};

class NumberInputIdea: public Idea
{
    Q_OBJECT
    Q_PROPERTY(double number READ number WRITE setNumber NOTIFY numberChanged)

public:
    NumberInputIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(QJsonObject const &p) override;

    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;
    double number() const;

signals:
    void numberChanged(double number);

public slots:
    void setNumber(double number);

private:
    std::shared_ptr<NumberDataType> m_number;
};

#endif //NUMBERINPUTIDEA_H





