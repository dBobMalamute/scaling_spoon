#ifndef CHANGECOLORSCOMMAND_H
#define CHANGECOLORSCOMMAND_H

#include <QUndoCommand>
#include <QJsonObject>
#include <QList>

class Corkboard;
class ChangeColorsCommand : public QUndoCommand
{
public:
    ChangeColorsCommand(QList<QUuid> ids, const QList<int> oldColors, const int newColor, Corkboard* corkboard);

    void undo() override;
    void redo() override;

private:
    QList<QUuid> m_ids;

    QList<int> m_oldColors;
    int m_newColor;

    Corkboard* m_corkboard;
};

#endif // CHANGECOLORCOMMAND_H
