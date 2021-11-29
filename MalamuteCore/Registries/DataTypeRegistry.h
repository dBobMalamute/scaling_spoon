#ifndef DATATYPEREGISTRY_H
#define DATATYPEREGISTRY_H

#include <unordered_map>

#include <QString>
#include <QColor>
#include "../InheritedClasses/Magic.h"

class DataTypeRegistry
{

public:
    template<typename DataTypeTemplate> void registerDataTypeAttributes()
    {
        if(Magic<DataTypeTemplate>::cast().id() != -1)
        {   //Data type registered multiple times.
            return;
        }

        Magic<DataTypeTemplate>::cast().setID(m_idCounter);
        m_idCounter++;

        QString name = Magic<DataTypeTemplate>::cast().name();
        m_dataTypeNames.append(name);

        QString colorString = Magic<DataTypeTemplate>::cast().color();
        m_dataTypeColors.append(QColor(colorString));

        QString abbreviation = Magic<DataTypeTemplate>::cast().abbreviation();
        m_dataTypeAbbreviations.append(abbreviation);

        QString description = Magic<DataTypeTemplate>::cast().description();
        m_dataTypeDescriptions.append(description);

        QString defaultDisplay = Magic<DataTypeTemplate>::cast().defaultDisplay();
        m_defaultDisplays.append(defaultDisplay);

        QString defaultSource = Magic<DataTypeTemplate>::cast().defaultSource();
        m_defaultSources.append(defaultSource);
    }

    QColor dataTypeColor(int id);
    QString dataTypeAbbreviation(int id);
    QString dataTypeDescription(int id);
    QString defaultDisplay(int id);
    QString defaultSource(int id);

private:
    QList<QString> m_dataTypeNames;
    QList<QColor> m_dataTypeColors;
    QList<QString> m_dataTypeAbbreviations;
    QList<QString> m_dataTypeDescriptions;
    QList<QString> m_defaultDisplays;
    QList<QString> m_defaultSources;

    int m_idCounter = 0;
};

#endif // DATATYPEREGISTRY_H
