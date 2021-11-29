#ifndef DELETESELECTIONCOMMAND_H
#define DELETESELECTIONCOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>
#include <QJsonArray>

class Corkboard;

class DeleteSelectionCommand: public QUndoCommand
{
public:
    DeleteSelectionCommand(const QJsonObject& obj, Corkboard* corkboard);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QJsonObject m_obj;
    QJsonArray m_extraWireArray;
};

#endif // REMOVESELECTIONCOMMAND_H
