#include "AddSelectionCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

AddSelectionCommand::AddSelectionCommand(const QJsonObject &obj, Corkboard *corkboard, QPointF center, bool changeUuids):
    m_corkboard(corkboard),
    m_obj(obj),
    m_center(center),
    m_changeUuids(changeUuids)
{

}

void AddSelectionCommand::undo()
{
    //Delete the wires.
    QJsonArray wiresJsonArray = m_obj["wires"].toArray();
    for(QJsonValueRef wire: wiresJsonArray)
    {
        m_corkboard->backend()->removeWire(QUuid(wire.toObject()["id"].toString()));
    }

    //Delete the casings
    QJsonArray casingsJsonArray = m_obj["casings"].toArray();
    for(QJsonValueRef casing: casingsJsonArray)
    {
        m_corkboard->backend()->removeCasingBackend(QUuid(casing.toObject()["id"].toString()));
    }

    //Delete the postIts
    QJsonArray postItsJsonArray = m_obj["postIts"].toArray();
    for(QJsonValueRef postIt : postItsJsonArray)
    {
        m_corkboard->removePostItBypassUndoStack(QUuid(postIt.toObject()["id"].toString()));
    }
}

void AddSelectionCommand::redo()
{
    if(m_changeUuids)
    {
        //The selection to be added. As well as the new arrays which will take their place.
        QJsonArray casingsJsonArray = m_obj["casings"].toArray();
        QJsonArray newCasingsJsonArray;
        QJsonArray wiresJsonArray = m_obj["wires"].toArray();
        QJsonArray newWiresJsonArray;
        QJsonArray postItsArray = m_obj["postIts"].toArray();
        QJsonArray newPostItsArray;

        QList<QUuid> oldCasingIds;
        QList<QUuid> newCasingIds;

        double top = 1e100; double bottom = -1e100;
        double left = 1e100; double right = -1e100;

        ///This code find the bounds of the selection to be added.
        for (QJsonValueRef casing : casingsJsonArray)
        {
            QJsonObject obj = casing.toObject();
            QJsonObject positionJson = obj["comp"].toObject();

            top = qMin(top, positionJson["y"].toDouble());
            bottom = qMax(bottom, positionJson["y"].toDouble() + 100.0);
            left = qMin(left,positionJson["x"].toDouble());
            right = qMax(right, positionJson["x"].toDouble() + 100.0);
        }
        for(QJsonValueRef postIt : postItsArray)
        {
            QJsonObject obj = postIt.toObject();
            top = qMin(top, obj["y"].toDouble());
            bottom = qMax(bottom, obj["y"].toDouble() + 100);
            left = qMin(left, obj["x"].toDouble());
            right = qMax(right, obj["x"].toDouble() + 100);
        }

        // Calculates how to move the selection.
        QPointF oldCenter = QPointF((left + right)/2.0,(top + bottom)/2.0);
        QPointF displacement = m_center - oldCenter;

        //Loop over the new casings.
        for (QJsonValueRef casing : casingsJsonArray)
        {
            QJsonObject obj = casing.toObject();

            // Change the Uuid, record the change.
            oldCasingIds.append(QUuid(obj["id"].toString()));
            QUuid newId = QUuid::createUuid();
            obj["id"] = newId.toString();
            newCasingIds.append(QUuid(obj["id"].toString()));

            //Shift the idea to the new position.
            QJsonObject positionJson = obj["comp"].toObject();
            positionJson["x"] = positionJson["x"].toDouble() + displacement.x();
            positionJson["y"] = positionJson["y"].toDouble() + displacement.y();
            obj["comp"] = positionJson;

            //Create the new idea. Store what is done for future undo/redo operations.
            m_corkboard->createCasingBypassUndoStack(obj);
            newCasingsJsonArray.append(obj);
        }

        // Set up the wires.
        for (QJsonValueRef wire : wiresJsonArray)
        {
            bool ok1 = false; bool ok2 = false;
            QJsonObject obj = wire.toObject();

            QUuid inId = obj["i_id"].toString();
            for(int i = 0; i < oldCasingIds.size(); i++)
            {
                if(oldCasingIds[i] == inId)
                {
                    ok1 = true;
                    obj["i_id"] = newCasingIds[i].toString();
                    break;
                }
            }

            QUuid outId = obj["o_id"].toString();
            for(int i = 0; i < oldCasingIds.size(); i++)
            {
                if(oldCasingIds[i] == outId)
                {
                    ok2 = true;
                    obj["o_id"] = newCasingIds[i].toString();
                    break;
                }
            }

            if(ok1 && ok2)
            {
                obj["id"] = QUuid::createUuid().toString();
                m_corkboard->createWireBypassUndoStack(obj);
                newWiresJsonArray.append(obj);
            }
        }

        //Set up the post its.
        for (QJsonValueRef postIt : postItsArray)
        {
            QJsonObject obj = postIt.toObject();
            obj["id"] = QUuid::createUuid().toString();
            obj["x"] = obj["x"].toDouble() + displacement.x();
            obj["y"] = obj["y"].toDouble() + displacement.y();
            m_corkboard->createPostItBypassUndoStack(obj);
            newPostItsArray.append(obj);
        }

        //Set up the new arrays.
        m_obj["casings"] = newCasingsJsonArray;
        m_obj["wires"] = newWiresJsonArray;
        m_obj["postIts"] = newPostItsArray;
        m_changeUuids = false;
    }
    else
    {
        // An additional undo operation might call this.
        QJsonArray casingsJsonArray = m_obj["casings"].toArray();
        QJsonArray wiresJsonArray = m_obj["wires"].toArray();
        QJsonArray postItsJsonArray = m_obj["postIts"].toArray();

        //Loop over and create new casings.
        for (QJsonValueRef casing : casingsJsonArray)
        {
            m_corkboard->createCasingBypassUndoStack(casing.toObject());
        }

        // loop over and create new wires.
        for (QJsonValueRef wire : wiresJsonArray)
        {
            m_corkboard->createWireBypassUndoStack(wire.toObject());
        }

        // loop over and create new post its.
        for(QJsonValueRef postIt : postItsJsonArray)
        {
            m_corkboard->createPostItBypassUndoStack(postIt.toObject());
        }
    }
}
