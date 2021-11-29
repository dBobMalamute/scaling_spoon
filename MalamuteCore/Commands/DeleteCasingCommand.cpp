#include "DeleteCasingCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

DeleteCasingCommand::DeleteCasingCommand(const QJsonObject &obj, Corkboard *corkboard) :
    m_corkboard(corkboard),
    m_obj(obj)
{

}

void DeleteCasingCommand::undo()
{
    //Recreate the casing.
    m_corkboard->createCasingBypassUndoStack(m_obj);

    //Recreate connections which were no longer valid.
    for (QJsonValueRef wire : m_extraWireArray)
    {
        m_corkboard->createWireBypassUndoStack(wire.toObject());
    }
}

void DeleteCasingCommand::redo()
{
    //Delete the idea, store a record of the extra deleted objects
    m_extraWireArray = QJsonArray();
    QJsonArray tempArray;

    tempArray = m_corkboard->backend()->removeCasingBackend(QUuid(m_obj["id"].toString()));
    for(int i = 0; i < tempArray.size(); i++)
    {
        m_extraWireArray.append(tempArray[i]);
    }
}
