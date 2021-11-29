#include "MinimizeCasingCommand.h"
#include "../Corkboard.h"

MinimizeCasingCommand::MinimizeCasingCommand(const QUuid &id, Corkboard *corkboard, bool oldState, bool actionNeeded) :
    m_corkboard(corkboard),
    m_id(id),
    m_oldState(oldState),
    m_actionNeeded(actionNeeded)
{

}

void MinimizeCasingCommand::undo()
{
    m_actionNeeded = true;
    m_corkboard->minimizeCasingFromCommand(m_id, m_oldState);
}

void MinimizeCasingCommand::redo()
{
    if(m_actionNeeded)
        m_corkboard->minimizeCasingFromCommand(m_id, !m_oldState);
}
