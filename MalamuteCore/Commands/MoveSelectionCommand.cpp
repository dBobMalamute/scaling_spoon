#include "MoveSelectionCommand.h"
#include "../Corkboard.h"

MoveSelectionCommand::MoveSelectionCommand(std::vector<QUuid> idList, std::vector<QPointF> oldPosList, std::vector<QPointF> newPosList,
                                   Corkboard *corkboard, bool actionNeeded) :
    m_corkboard(corkboard),
    m_idList(idList),
    m_oldPosList(oldPosList),
    m_newPosList(newPosList),
    m_actionNeeded(actionNeeded)
{

}

void MoveSelectionCommand::undo()
{
    m_actionNeeded = true;
    for (unsigned int i = 0; i < m_idList.size(); i++)
    {
        m_corkboard->moveItemFromCommand(m_idList[i],m_oldPosList[i]);
    }
}

void MoveSelectionCommand::redo()
{
    if(m_actionNeeded)
    {
        for (unsigned int i = 0; i < m_idList.size(); i++)
        {
            m_corkboard->moveItemFromCommand(m_idList[i],m_newPosList[i]);
        }
    }
}
