#ifndef CHANGEPARAMSCOMMAND_H
#define CHANGEPARAMSCOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>
#include <QUuid>

class Corkboard;

class ChangeParamsCommand: public QUndoCommand
{
public:
    ChangeParamsCommand(QUuid id, QJsonObject oldParams, QJsonObject newParams,
                            Corkboard* corkboard, bool actionNeeded);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QUuid m_id;
    QJsonObject m_oldParams;
    QJsonObject m_newParams;
    bool m_actionNeeded;
};

#endif // CHANGEPARAMSCOMMAND_H
