#include "AddCasingCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

AddCasingCommand::AddCasingCommand(const QJsonObject& obj,Corkboard* corkboard) :
    m_corkboard(corkboard),
    m_obj(obj)
{
}

void AddCasingCommand::undo()
{
    m_corkboard->backend()->removeCasingBackend(QUuid(m_obj["id"].toString()));
}

void AddCasingCommand::redo()
{
    m_corkboard->createCasingBypassUndoStack(m_obj);
}
