#ifndef TANGENTIDEA_H
#define TANGENTIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct TangentAttributes : public IdeaAttributes
{
    TangentAttributes();
};

class TangentIdea : public Idea
{
    Q_OBJECT
    Q_PROPERTY(bool degreeMode READ degreeMode WRITE setDegreeMode NOTIFY degreeModeChanged)

public:
    TangentIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    bool degreeMode() const;

public slots:
    void setDegreeMode(bool degreeMode);

signals:
    void degreeModeChanged(bool degreeMode);
    void displayNumbers(double input, double output);
    void clearNumbers();

private:
    std::weak_ptr<NumberDataType> m_input;
    std::shared_ptr<NumberDataType> m_result;

    void validate();
    void compute(double n);
    bool m_degreeMode;
};

#endif // TANGENTIDEA_H
