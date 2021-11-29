#include "DeleteSelectionCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

DeleteSelectionCommand::DeleteSelectionCommand(const QJsonObject &obj, Corkboard *corkboard) :
    m_corkboard(corkboard),
    m_obj(obj)
{

}

void DeleteSelectionCommand::undo()
{
    //Recreate the casings.
    QJsonArray casingsJsonArray = m_obj["casings"].toArray();
    for(QJsonValueRef casing: casingsJsonArray)
    {
        m_corkboard->createCasingBypassUndoStack(casing.toObject());
    }

    //Recreate the selected connections.
    QJsonArray wiresJsonArray = m_obj["wires"].toArray();
    for(QJsonValueRef wire: wiresJsonArray)
    {
        m_corkboard->createWireBypassUndoStack(wire.toObject());
    }

    //Recreate connections which were no longer valid.
    for (QJsonValueRef wire : m_extraWireArray)
    {
        m_corkboard->createWireBypassUndoStack(wire.toObject());
    }

    //Recreate the postIts
    QJsonArray postItsJsonArray = m_obj["postIts"].toArray();
    for(QJsonValueRef postIt : postItsJsonArray)
    {
        m_corkboard->createPostItBypassUndoStack(postIt.toObject());
    }
}

void DeleteSelectionCommand::redo()
{
    //Delete the connections.

    QJsonArray wiresJsonArray = m_obj["wires"].toArray();
    for(QJsonValueRef wire: wiresJsonArray)
    {
        m_corkboard->backend()->removeWire(QUuid(wire.toObject()["id"].toString()));
    }

    //Delete the ideas, store a record of the extra deleted objects
    m_extraWireArray = QJsonArray();
    QJsonArray tempArray;
    QJsonArray casingsJsonArray = m_obj["casings"].toArray();
    for(QJsonValueRef casing: casingsJsonArray)
    {
        tempArray = m_corkboard->backend()->removeCasingBackend(QUuid(casing.toObject()["id"].toString()));
        for(int i = 0; i < tempArray.size(); i++)
        {
            m_extraWireArray.append(tempArray[i]);
        }
    }

    //Delete the postIts.
    QJsonArray postItsJsonArray = m_obj["postIts"].toArray();
    for(QJsonValueRef postIt: postItsJsonArray)
    {
        m_corkboard->removePostItBypassUndoStack(QUuid(postIt.toObject()["id"].toString()));
    }
}
