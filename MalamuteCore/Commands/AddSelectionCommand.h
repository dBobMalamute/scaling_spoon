#ifndef ADDSELECTIONCOMMAND_H
#define ADDSELECTIONCOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>
#include <QPointF>

class Corkboard;

class AddSelectionCommand: public QUndoCommand
{
public:
    AddSelectionCommand(const QJsonObject& obj, Corkboard* corkboard, QPointF center, bool changeUuids);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QJsonObject m_obj;
    QPointF m_center;
    bool m_changeUuids;
};

#endif // ADDSELECTIONCOMMAND_H
