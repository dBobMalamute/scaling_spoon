#ifndef DELETECASINGCOMMAND_H
#define DELETECASINGCOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>
#include <QJsonArray>

class Corkboard;

class DeleteCasingCommand: public QUndoCommand
{
public:
    DeleteCasingCommand(const QJsonObject& obj, Corkboard* corkboard);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QJsonObject m_obj;
    QJsonArray m_extraWireArray;
};

#endif // DELETECASINGCOMMAND_H
