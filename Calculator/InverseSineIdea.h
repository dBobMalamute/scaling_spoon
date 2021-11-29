#ifndef INVERSESINEIDEA_H
#define INVERSESINEIDEA_H

#include <QObject>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct InverseSineAttributes : public IdeaAttributes
{
    InverseSineAttributes();
};

class InverseSineIdea : public Idea
{
    Q_OBJECT
    Q_PROPERTY(bool degreeMode READ degreeMode WRITE setDegreeMode NOTIFY degreeModeChanged)

public:
    InverseSineIdea();
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

private:
    std::weak_ptr<NumberDataType> m_input;
    std::shared_ptr<NumberDataType> m_result;

    void validate();
    void compute(double n);
    bool m_degreeMode;
};

#endif // INVERSESINEIDEA_H
