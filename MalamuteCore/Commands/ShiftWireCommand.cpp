#include "ShiftWireCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

ShiftWireCommand::ShiftWireCommand(const QJsonObject &oldWire, const QJsonObject &newWire,
                                               Corkboard *corkboard, bool actionNeeded) :
    m_corkboard(corkboard),
    m_oldWire(oldWire),
    m_newWire(newWire),
    m_actionNeeded(actionNeeded)
{

}

void ShiftWireCommand::undo()
{
    // Always remember to unplug the old wires before adding new ones in.
    // I just spent 5 hours before figuring this out...
    // I have brought shame to the electrical engineer, my grandpa bob,
    // who gave me the nickname dBob. Time for sleep.
    m_corkboard->backend()->removeWire(QUuid(m_newWire["id"].toString()));
    m_corkboard->createWireBypassUndoStack(m_oldWire);
}

void ShiftWireCommand::redo()
{
    if(m_actionNeeded)
    {
        m_corkboard->backend()->removeWire(QUuid(m_oldWire["id"].toString()));
        m_corkboard->createWireBypassUndoStack(m_newWire);
    }
    m_actionNeeded = true;
}
