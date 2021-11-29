#ifndef RESIZECASINGCOMMAND_H
#define RESIZECASINGCOMMAND_H

#include <QUndoCommand>
#include <QUuid>
#include <QPointF>

class Corkboard;

class ResizeCasingCommand : public QUndoCommand
{
public:
    ResizeCasingCommand(const QUuid& id, Corkboard* corkboard, QPointF oldSize, QPointF newSize);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QUuid m_id;
    QPointF m_oldSize;
    QPointF m_newSize;
    bool m_actionNeeded;
};

#endif // RESIZECASINGCOMMAND_H
