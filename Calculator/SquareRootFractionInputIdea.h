#ifndef SQUAREROOTFRACTIONINPUTIDEA_H
#define SQUAREROOTFRACTIONINPUTIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct SquareRootFractionInputAttributes : public IdeaAttributes
{
    SquareRootFractionInputAttributes();
};

class SquareRootFractionInputIdea: public Idea
{
    Q_OBJECT

    Q_PROPERTY(double numerator READ numerator WRITE setNumerator NOTIFY numeratorChanged)
    Q_PROPERTY(double denominator READ denominator WRITE setDenominator NOTIFY denominatorChanged)

public:
    SquareRootFractionInputIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(QJsonObject const &p) override;

    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    double numerator() const;
    double denominator() const;

signals:
    void numeratorChanged(double numerator);
    void denominatorChanged(double denominator);

public slots:
    void setNumerator(double numerator);
    void setDenominator(double denominator);

private:
    std::shared_ptr<NumberDataType> m_number;

    double m_numerator;
    double m_denominator;

    void validate();
};

#endif //SQUAREROOTFRACTIONINPUTIDEA_H





