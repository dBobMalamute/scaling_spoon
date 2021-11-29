#include "DataTypeRegistry.h"

QColor DataTypeRegistry::dataTypeColor(int id)
{
    return m_dataTypeColors.at(id);
}

QString DataTypeRegistry::dataTypeAbbreviation(int id)
{
    return m_dataTypeAbbreviations.at(id);
}

QString DataTypeRegistry::dataTypeDescription(int id)
{
    return m_dataTypeDescriptions.at(id);
}

QString DataTypeRegistry::defaultDisplay(int id)
{
    return m_defaultDisplays.at(id);
}

QString DataTypeRegistry::defaultSource(int id)
{
    return m_defaultSources.at(id);
}
