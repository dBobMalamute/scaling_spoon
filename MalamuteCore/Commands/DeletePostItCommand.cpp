#include "DeletePostItCommand.h"
#include "../Corkboard.h"

DeletePostItCommand::DeletePostItCommand(const QJsonObject &obj, Corkboard *corkboard) :
    m_corkboard(corkboard),
    m_obj(obj)
{

}

void DeletePostItCommand::undo()
{
    m_corkboard->createPostItBypassUndoStack(m_obj);
}

void DeletePostItCommand::redo()
{
    m_corkboard->removePostItBypassUndoStack(QUuid(m_obj["id"].toString()));
}
