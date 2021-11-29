//Copyright (c) 2017, Dmitry Pinaev
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are
//met:

//    * Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
//copyright notice, this list of conditions and the following disclaimer
//in the documentation and/or other materials provided with the
//distribution.
//    * Neither the name of copyright holder, nor the names of its
//contributors may be used to endorse or promote products derived from
//this software without specific prior written permission.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This class was changed slightly.

#ifndef IDEAREGISTRY_H
#define IDEAREGISTRY_H

#include <unordered_map>
#include "../InheritedClasses/Idea.h"
#include "../InheritedClasses/DataType.h"

/// Class uses map for storing ideas (name, idea)
class IdeaRegistry
{
public:

    template<typename IdeaType> void registerIdea()
    {
        auto idea = std::make_unique<IdeaType>();

        std::function<std::unique_ptr<Idea>()> constructorDispatchFunc = []()
        {
            return std::make_unique<IdeaType>();
        };

        QString listName = idea->attributes().listName();
        QString iconPath = idea->attributes().iconPath();

        if (m_dispatchTable.count(listName) == 0)
        {
            m_dispatchTable[listName] = std::move(constructorDispatchFunc);
            m_iconPaths[listName] = iconPath;
        }
    }

    std::unique_ptr<Idea> createIdea (QString const &listName);
    QString iconPath(QString listName);
    QStringList registeredIdeaNames();

private:
    std::unordered_map<QString, std::function<std::unique_ptr<Idea>()>> m_dispatchTable;
    std::unordered_map<QString, QString> m_iconPaths;
};

#endif // IDEAREGISTRY_H
