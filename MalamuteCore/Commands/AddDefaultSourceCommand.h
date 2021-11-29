#ifndef ADDDEFAULTSOURCECOMMAND_H
#define ADDDEFAULTSOURCECOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>

class Corkboard;

class AddDefaultSourceCommand: public QUndoCommand
{
public:
    AddDefaultSourceCommand(const QJsonObject& casingObj, const QJsonObject& wireObj, Corkboard* corkboard);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QJsonObject m_casingObj;
    QJsonObject m_wireObj;
};

#endif // ADDDEFAULTSOURCECOMMAND_H
