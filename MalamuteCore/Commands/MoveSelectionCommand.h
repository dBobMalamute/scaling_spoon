#ifndef MOVESELECTIONCOMMAND_H
#define MOVESELECTIONCOMMAND_H

#include <QUndoCommand>

class Corkboard;

class MoveSelectionCommand: public QUndoCommand
{
public:
    MoveSelectionCommand(std::vector<QUuid> idList, std::vector<QPointF> oldPosList, std::vector<QPointF> newPosList,
                     Corkboard* corkboard, bool actionNeeded);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    std::vector<QUuid> m_idList;
    std::vector<QPointF> m_oldPosList;
    std::vector<QPointF> m_newPosList;
    bool m_actionNeeded;
};

#endif // MOVESELECTIONCOMMAND_H
