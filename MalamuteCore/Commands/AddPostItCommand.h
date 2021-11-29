#ifndef ADDPOSTITCOMMAND_H
#define ADDPOSTITCOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>

class Corkboard;

class AddPostItCommand : public QUndoCommand
{
public:
    AddPostItCommand(const QJsonObject& obj, Corkboard* corkboard);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    QJsonObject m_obj;
};

#endif // ADDPOSTITCOMMAND_H
