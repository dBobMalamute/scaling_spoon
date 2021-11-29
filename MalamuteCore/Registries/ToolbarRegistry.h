#ifndef TOOLBARREGISTRY_H
#define TOOLBARREGISTRY_H

#include <unordered_map>

#include <QString>
#include <QStringList>

class ToolbarRegistry
{
public:
    void registerToolbar(QString collectionName, QStringList orderedIdeas)
    {
        if(m_ideaCollectionMap.count(collectionName) == 0)
        {
            m_ideaCollectionMap[collectionName] = orderedIdeas;
        }
    }

    QStringList toolbarList();
    QStringList toolbarIdeas(QString name);

private:
    std::unordered_map<QString, QStringList> m_ideaCollectionMap;
};

#endif // TOOLBARREGISTRY_H
