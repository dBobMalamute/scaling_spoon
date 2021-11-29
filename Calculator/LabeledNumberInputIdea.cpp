#include "LabeledNumberInputIdea.h"

#include <QJsonValue>
#include <QDebug>

LabeledNumberInputAttributes::LabeledNumberInputAttributes()
{
    m_listName = "Labeled Number Input";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This idea allows the user to enter a number and a label, When selected, press enter to update the number"
                  " and propogate the change to other ideas.";
    m_iconPath = "qrc:/Icons/LabeledNumberInput.png";

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsOut << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/LabeledNumberInputIdea.qml";
}

LabeledNumberInputIdea::LabeledNumberInputIdea()
{
    m_number = std::make_shared<NumberDataType>();
    setNumber(m_number->number());
    m_text = "Label";

    setStatus(IdeaStatus::Working, "Sourcing 0", "");
}

const IdeaAttributes LabeledNumberInputIdea::attributes()
{
    return Magic<LabeledNumberInputAttributes>::cast();
}

QJsonObject LabeledNumberInputIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["num"] = m_number->number();
    saveJson["txt"] = m_text;

    return saveJson;
}

void LabeledNumberInputIdea::load(QJsonObject const &p)
{
    QJsonValue v = p["num"];

    if (!v.isUndefined())
    {
        m_number = std::make_shared<NumberDataType>(v.toDouble());
        emit numberChanged();
        emit newData(0);
    }

    QJsonValue v2 = p["txt"];

    if (!v2.isUndefined())
    {
        m_text = v2.toString();
        emit textChanged();
    }

}

std::shared_ptr<DataType> LabeledNumberInputIdea::dataOut(PlugNumber)
{
    return m_number;
}

void LabeledNumberInputIdea::onNewDataIn(std::shared_ptr<DataType>, int)
{

}

double LabeledNumberInputIdea::number() const
{
    return m_number->number();
}

QString LabeledNumberInputIdea::text() const
{
    return m_text;
}

void LabeledNumberInputIdea::setNumber(double number)
{
    if(!qFuzzyCompare(m_number->number(),number))
    {
        QJsonObject oldParams = this->save();
        m_number = std::make_shared<NumberDataType>(number);
        emit newData(0);

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);
        setStatus(IdeaStatus::Working, "Sourcing: " + QString::number(m_number->number()), "");
    }
}

void LabeledNumberInputIdea::setText(QString text)
{
    if (m_text == text)
        return;
    else
    {
        QJsonObject oldParams = this->save();
        m_text = text;
        emit textChanged();

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);
    }
}
