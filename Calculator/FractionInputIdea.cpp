#include "FractionInputIdea.h"

#include <QJsonValue>
#include <QDebug>

FractionInputAttributes::FractionInputAttributes()
{
    m_listName = "Fraction Input";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This idea allows the user to enter a number. When selected, press enter to update the number"
                  " and propogate the change to other ideas.";
    m_iconPath = "qrc:/Icons/FractionInput.png";

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsOut << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/FractionInputIdea.qml";
}

FractionInputIdea::FractionInputIdea() :
    m_numerator(2.0),
    m_denominator(5.0)
{
    m_number = std::make_shared<NumberDataType>(0.4);

    setStatus(IdeaStatus::Working, "Sourcing: 0", "");
}

const IdeaAttributes FractionInputIdea::attributes()
{
    return Magic<FractionInputAttributes>::cast();
}

QJsonObject FractionInputIdea::save() const
{
    QJsonObject ideaJson;
    ideaJson["n"] = m_numerator;
    ideaJson["d"] = m_denominator;
    return ideaJson;
}

void FractionInputIdea::load(QJsonObject const &p)
{
    QJsonValue v = p["n"];

    if (!v.isUndefined())
    {
        m_numerator = v.toDouble();
        emit numeratorChanged(m_numerator);
    }

    v = p["d"];
    if(!v.isUndefined())
    {
        m_denominator = v.toDouble();
        emit denominatorChanged(m_denominator);
    }

    validate();
}

std::shared_ptr<DataType> FractionInputIdea::dataOut(PlugNumber)
{
    return m_number;
}

double FractionInputIdea::numerator() const
{
    return m_numerator;
}

double FractionInputIdea::denominator() const
{
    return m_denominator;
}

void FractionInputIdea::setNumerator(double numerator)
{
    if (qFuzzyCompare(m_numerator, numerator))
        return;

    QJsonObject oldParams;
    oldParams["n"] = m_numerator;

    m_numerator = numerator;
    emit numeratorChanged(m_numerator);

    QJsonObject newParams;
    newParams["n"] = m_numerator;
    reportParamsChanged(oldParams, newParams);

    validate();
}

void FractionInputIdea::setDenominator(double denominator)
{
    if (qFuzzyCompare(m_denominator, denominator))
        return;

    QJsonObject oldParams;
    oldParams["d"] = m_denominator;

    m_denominator = denominator;
    emit denominatorChanged(m_denominator);

    QJsonObject newParams;
    newParams["d"] = m_denominator;
    reportParamsChanged(oldParams, newParams);

    validate();
}

void FractionInputIdea::validate()
{
    if(qFuzzyIsNull(m_denominator))
    {
        setStatus(IdeaStatus::Error, "Divide by 0", "Division by 0 is not allowed");
        return;
    }

    double num = m_numerator / m_denominator;
    setStatus(IdeaStatus::Working, "Working", "Sourcing: " + QString::number(num));
    m_number = std::make_shared<NumberDataType>(num);
    emit newData(0);
}
