#ifndef LABELEDNUMBERDISPLAYIDEA_H
#define LABELEDNUMBERDISPLAYIDEA_H

#include <QObject>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct LabeledNumberDisplayAttributes : public IdeaAttributes
{
    LabeledNumberDisplayAttributes();
};

class LabeledNumberDisplayIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    LabeledNumberDisplayIdea();
    ~LabeledNumberDisplayIdea() override;

    const IdeaAttributes attributes() override;
    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;
    void onNewDataIn(std::shared_ptr<DataType> data, int) override;

    QString text() const;

public slots:
    void setText(QString text);

signals:
    void displayNumber(QString value);
    void textChanged();

private:
    QString m_text;
};

#endif //LABELEDNUMBERDISPLAYIDEA_H
