#include "IdeaRegistry.h"

std::unique_ptr<Idea> IdeaRegistry::createIdea(QString const &listName)
{
    auto it = m_dispatchTable.find(listName);
    if (it != m_dispatchTable.end())
    {
        return it->second();
    }
    return nullptr;
}

QString IdeaRegistry::iconPath(QString listName)
{
    return m_iconPaths[listName];
}

QStringList IdeaRegistry::registeredIdeaNames()
{
    QStringList ideaList;

    for(auto idea : m_dispatchTable)
    {
        ideaList.append(idea.first);
    }
    return ideaList;
}
