#include "ToolbarRegistry.h"

QStringList ToolbarRegistry::toolbarList()
{
    QStringList collectionList;

    for(auto collection : m_ideaCollectionMap)
    {
        collectionList.append(collection.first);
    }
    return collectionList;
}

QStringList ToolbarRegistry::toolbarIdeas(QString name)
{
    auto it = m_ideaCollectionMap.find(name);
    if(it != m_ideaCollectionMap.end())
    {
        return m_ideaCollectionMap[it->first];
    }
    return QStringList();
}

