#include "ChangeParamsCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

ChangeParamsCommand::ChangeParamsCommand(QUuid id, QJsonObject oldParams, QJsonObject newParams,
                                                 Corkboard *corkboard, bool actionNeeded) :
    m_corkboard(corkboard),
    m_id(id),
    m_oldParams(oldParams),
    m_newParams(newParams),
    m_actionNeeded(actionNeeded)
{

}

void ChangeParamsCommand::undo()
{
    m_actionNeeded = true;
    m_corkboard->backend()->getCasing(m_id)->idea()->load(m_oldParams);
}

void ChangeParamsCommand::redo()
{
    if(m_actionNeeded)
    {
        m_corkboard->backend()->getCasing(m_id)->idea()->load(m_newParams);
    }
}
