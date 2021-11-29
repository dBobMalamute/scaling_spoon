#ifndef ADDWIRECOMMAND_H
#define ADDWIRECOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>

class Corkboard;

class AddWireCommand: public QUndoCommand
{
public:
    AddWireCommand(const QJsonObject& obj,Corkboard* corkboard, bool actionNeeded);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QJsonObject m_obj;
    bool m_actionNeeded;
};

#endif // ADDWIRECOMMAND_H
