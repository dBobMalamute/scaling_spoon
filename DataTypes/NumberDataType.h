#ifndef NUMBERDATATYPE_H
#define NUMBERDATATYPE_H

#include <QVariant>
#include <QString>
#include <QColor>
#include <QtPlugin>

#include "../MalamuteCore/InheritedClasses/DataType.h"

struct NumberDataTypeAttributes : public DataTypeAttributes
{
    NumberDataTypeAttributes();
};

class NumberDataType : public DataType
{
public:
    explicit NumberDataType(double const number = 0.0); //Default value of 0.

    DataTypeAttributes getAttributes() override;
    double number();

private:
    double m_number;
};

#endif
