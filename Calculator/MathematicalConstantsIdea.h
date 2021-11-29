#ifndef MATHEMATICALCONSTANTSIDEA_H
#define MATHEMATICALCONSTANTSIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct MathematicalConstantsAttributes : public IdeaAttributes
{
    MathematicalConstantsAttributes();
};

class MathematicalConstantsIdea: public Idea
{
    Q_OBJECT

public:
    MathematicalConstantsIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(QJsonObject const &p) override;

    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

signals:
    void indexChanged(int i);

public slots:
    void setIndex(int i);
    int getIndex();

private:
    void setNumber();
    std::shared_ptr<NumberDataType> m_number;
    int m_index = 0;
};

#endif // MATHEMATICALCONSTANTSSOURCEIDEA_H





