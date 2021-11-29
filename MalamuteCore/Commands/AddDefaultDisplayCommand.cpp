#include "AddDefaultDisplayCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

AddDefaultDisplayCommand::AddDefaultDisplayCommand(const QJsonObject& casingObj, const QJsonObject &wireObj, Corkboard* corkboard) :
    m_corkboard(corkboard),
    m_casingObj(casingObj),
    m_wireObj(wireObj)
{

}

void AddDefaultDisplayCommand::undo()
{
    m_corkboard->backend()->removeWire(QUuid(m_wireObj["id"].toString()));
    m_corkboard->backend()->removeCasingBackend(QUuid(m_casingObj["id"].toString()));
}

void AddDefaultDisplayCommand::redo()
{
    m_corkboard->createCasingBypassUndoStack(m_casingObj);
    m_corkboard->createWireBypassUndoStack(m_wireObj);
}
