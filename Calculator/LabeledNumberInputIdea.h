#ifndef LABELEDNUMBERINPUTIDEA_H
#define LABELEDNUMBERINPUTIDEA_H

#include <QObject>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"

struct LabeledNumberInputAttributes : public IdeaAttributes
{
    LabeledNumberInputAttributes();
};

class LabeledNumberInputIdea: public Idea
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(double number READ number WRITE setNumber NOTIFY numberChanged)

public:
    LabeledNumberInputIdea();
    ~LabeledNumberInputIdea() override {}
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(QJsonObject const &p) override;

    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;
    void onNewDataIn(std::shared_ptr<DataType>, int) override;

    QString text() const;
    double number() const;

signals:
    void textChanged();
    void numberChanged();

public slots:
    void setText(QString text);
    void setNumber(double number);

private:
    QString m_text;
    std::shared_ptr<NumberDataType> m_number;

};

#endif //LABELEDNUMBERINPUTIDEA_H





