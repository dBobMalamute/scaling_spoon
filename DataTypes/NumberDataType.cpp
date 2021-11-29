#include "NumberDataType.h"

NumberDataTypeAttributes::NumberDataTypeAttributes()
{
    m_name = "Decimal Number";
    m_color = "#2d8ccb";
    m_abbreviation = "#";
    m_description = "The number data types stores a double precision number";
    m_authors << "dBob";

    m_defaultDisplay = "Number Display";
    m_defaultSource = "Number Input";
}

DataTypeAttributes NumberDataType::getAttributes()
{
    return Magic<NumberDataTypeAttributes>::cast();
}


NumberDataType::NumberDataType(const double number)
{
    m_number = number;
}


double NumberDataType::number()
{
    return m_number;
}



