#include "ChangeColorsCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

ChangeColorsCommand::ChangeColorsCommand(QList<QUuid> ids, const QList<int> oldColors,
                                         const int newColor, Corkboard* corkboard):
    m_ids(ids),
    m_oldColors(oldColors),
    m_newColor(newColor),
    m_corkboard(corkboard)
{

}

void ChangeColorsCommand::undo()
{
    for(int i = 0; i < m_ids.length(); i++)
    {
        m_corkboard->backend()->getCasing(
                    m_ids.at(i))->casing().setPredefinedColor(m_oldColors.at(i));
    }
}

void ChangeColorsCommand::redo()
{
    for(int i = 0; i < m_ids.length(); i++)
    {
        m_corkboard->backend()->getCasing(
                    m_ids.at(i))->casing().setPredefinedColor(m_newColor);
    }
}
