#include "DataTypeAttributes.h"

QString DataTypeAttributes::name() const
{
    return m_name;
}

QString DataTypeAttributes::color() const
{
    return m_color;
}

QString DataTypeAttributes::abbreviation() const
{
    return m_abbreviation;
}

QString DataTypeAttributes::description() const
{
    return m_description;
}

QStringList DataTypeAttributes::authors() const
{
    return m_authors;
}

QString DataTypeAttributes::defaultDisplay() const
{
    return m_defaultDisplay;
}

QString DataTypeAttributes::defaultSource() const
{
    return m_defaultSource;
}

int DataTypeAttributes::id() const
{
    return m_id;
}

void DataTypeAttributes::setID(int value)
{
    m_id = value;
}
