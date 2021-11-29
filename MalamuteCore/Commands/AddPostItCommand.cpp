#include "AddPostItCommand.h"
#include "../Corkboard.h"

AddPostItCommand::AddPostItCommand(const QJsonObject &obj, Corkboard *corkboard) :
    m_corkboard(corkboard),
    m_obj(obj)
{

}

void AddPostItCommand::undo()
{
    m_corkboard->removePostItBypassUndoStack(QUuid(m_obj["id"].toString()));
}

void AddPostItCommand::redo()
{
    m_corkboard->createPostItBypassUndoStack(m_obj);
}
