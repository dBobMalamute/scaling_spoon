#include "NumberInputIdea.h"

#include <QJsonValue>
#include <QDebug>

NumberInputAttributes::NumberInputAttributes()
{
    m_listName = "Number Input";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This idea allows the user to enter a number. When selected, press enter to update the number"
                  " and propogate the change to other ideas.";
    m_iconPath = "qrc:/Icons/NumberInput.png";

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsOut << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/NumberInputIdea.qml";
}

NumberInputIdea::NumberInputIdea()
{
    m_number = std::make_shared<NumberDataType>();
    setNumber(m_number->number());
    setStatus(IdeaStatus::Working, "Sourcing: 0", "");
}

const IdeaAttributes NumberInputIdea::attributes()
{
    return Magic<NumberInputAttributes>::cast();
}

QJsonObject NumberInputIdea::save() const
{
    QJsonObject ideaJson;
    ideaJson["num"] = m_number->number();
    return ideaJson;
}

void NumberInputIdea::load(QJsonObject const &p)
{
    QJsonValue v = p["num"];

    if (!v.isUndefined())
    {
        m_number = std::make_shared<NumberDataType>(v.toDouble());
        emit numberChanged(v.toDouble());
    }
    emit newData(0);
}

std::shared_ptr<DataType> NumberInputIdea::dataOut(PlugNumber)
{
    return m_number;
}

double NumberInputIdea::number() const
{
    return m_number->number();
}

void NumberInputIdea::setNumber(double number)
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
