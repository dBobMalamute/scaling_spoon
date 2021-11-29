#include "ResizeCasingCommand.h"
#include "../Corkboard.h"

ResizeCasingCommand::ResizeCasingCommand(const QUuid &id, Corkboard *corkboard, QPointF oldSize, QPointF newSize) :
    m_corkboard(corkboard),
    m_id(id),
    m_oldSize(oldSize),
    m_newSize(newSize),
    m_actionNeeded(false)
{

}

void ResizeCasingCommand::undo()
{
    m_actionNeeded = true;
    m_corkboard->resizeCasingFromCommand(m_id, m_oldSize);
}

void ResizeCasingCommand::redo()
{
    if(m_actionNeeded)
        m_corkboard->resizeCasingFromCommand(m_id, m_newSize);
}
