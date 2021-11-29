#ifndef NUMBERSDATATYPE_H
#define NUMBERSDATATYPE_H

#include <QVariant>
#include <QString>
#include <QColor>

#include "../MalamuteCore/InheritedClasses/DataType.h"

struct NumbersDataTypeAttributes : public DataTypeAttributes
{
    NumbersDataTypeAttributes();
};

class NumbersDataType : public DataType
{
public:
    NumbersDataType(QList<double> numbers);

    DataTypeAttributes getAttributes() override;

    QList<double> numbers();
    QString toString();

private:
    QList<double> m_numbers;
};

#endif
