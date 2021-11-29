#include "AddWireCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

AddWireCommand::AddWireCommand(const QJsonObject& obj,Corkboard* corkboard, bool actionNeeded) :
    m_corkboard(corkboard),
    m_obj(obj),
    m_actionNeeded(actionNeeded)
{
}

void AddWireCommand::undo()
{
    m_actionNeeded = true;
    m_corkboard->backend()->removeWire(QUuid(m_obj["id"].toString()));
}

void AddWireCommand::redo()
{
    if(m_actionNeeded)
    {
        m_corkboard->createWireBypassUndoStack(m_obj);
    }
}
