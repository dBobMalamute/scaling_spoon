#ifndef DELETEPOSTITCOMMAND_H
#define DELETEPOSTITCOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>

class Corkboard;

class DeletePostItCommand : public QUndoCommand
{
public:
    DeletePostItCommand(const QJsonObject& obj, Corkboard* corkboard);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QJsonObject m_obj;
};

#endif // DELETEPOSTITCOMMAND_H
