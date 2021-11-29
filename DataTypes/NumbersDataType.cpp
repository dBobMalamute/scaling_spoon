#include "NumbersDataType.h"

NumbersDataTypeAttributes::NumbersDataTypeAttributes()
{
    m_name = "Number List";
    m_color = "darkblue";
    m_abbreviation = "Nums";
    m_description = "Stores a list of numbers. Useful for passing to other ideas.";
    m_authors << "dBob";

    m_defaultDisplay = "Numbers Display";
    m_defaultSource = "Numbers Input";
}

DataTypeAttributes NumbersDataType::getAttributes()
{
    return Magic<NumbersDataTypeAttributes>::cast();
}

NumbersDataType::NumbersDataType(QList<double> numbers)
{
    m_numbers.clear();
    m_numbers = numbers;
}

QList<double> NumbersDataType::numbers()
{
    return m_numbers;
}

QString NumbersDataType::toString()
{
    if(m_numbers.size() == 0)
        return "";

    QString returnValue = "";
    for(int i = 0; i < m_numbers.length(); i++)
    {
        returnValue.append(QString::number(m_numbers[i]));
        returnValue.append(", ");
    }
    returnValue.chop(2);
    return returnValue;
}

