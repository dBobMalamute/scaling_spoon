#include "DeleteWireCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

DeleteWireCommand::DeleteWireCommand(const QJsonObject& obj, Corkboard* corkboard) :
    m_corkboard(corkboard),
    m_obj(obj)
{

}

void DeleteWireCommand::undo()
{
    m_corkboard->createWireBypassUndoStack(m_obj);
}

void DeleteWireCommand::redo()
{
    m_corkboard->backend()->removeWire(QUuid(m_obj["id"].toString()));
}
