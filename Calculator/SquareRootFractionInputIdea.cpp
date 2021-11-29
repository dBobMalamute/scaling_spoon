#include "SquareRootFractionInputIdea.h"

#include <QJsonValue>
#include <QDebug>
#include <QtMath>

SquareRootFractionInputAttributes::SquareRootFractionInputAttributes()
{
    m_listName = "Square Root Fraction Input";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This idea allows the user to enter a number. When selected, press enter to update the number"
                  " and propogate the change to other ideas.";
    m_iconPath = "qrc:/Icons/SquareRootFractionInput.png";

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsOut << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/SquareRootFractionInputIdea.qml";
}

SquareRootFractionInputIdea::SquareRootFractionInputIdea() :
    m_numerator(2.0),
    m_denominator(5.0)
{
    m_number = std::make_shared<NumberDataType>(0.632455532);

    setStatus(IdeaStatus::Working, "Sourcing: 0", "");
}

const IdeaAttributes SquareRootFractionInputIdea::attributes()
{
    return Magic<SquareRootFractionInputAttributes>::cast();
}

QJsonObject SquareRootFractionInputIdea::save() const
{
    QJsonObject ideaJson;
    ideaJson["n"] = m_numerator;
    ideaJson["d"] = m_denominator;
    return ideaJson;
}

void SquareRootFractionInputIdea::load(QJsonObject const &p)
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

std::shared_ptr<DataType> SquareRootFractionInputIdea::dataOut(PlugNumber)
{
    return m_number;
}

double SquareRootFractionInputIdea::numerator() const
{
    return m_numerator;
}

double SquareRootFractionInputIdea::denominator() const
{
    return m_denominator;
}

void SquareRootFractionInputIdea::setNumerator(double numerator)
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

void SquareRootFractionInputIdea::setDenominator(double denominator)
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

void SquareRootFractionInputIdea::validate()
{
    if(qFuzzyIsNull(m_denominator))
    {
        setStatus(IdeaStatus::Error, "Divide by 0", "Division by 0 is not allowed");
        return;
    }
    else if((m_numerator < 0 && m_denominator > 0) || (m_denominator < 0 && m_numerator > 0))
    {
        setStatus(IdeaStatus::Error, "Complex Number", "This box assumes you do not want complex numbers");
        return;
    }
    else
    {
        double num = qSqrt(m_numerator / m_denominator);
        setStatus(IdeaStatus::Working, "Working", "Sourcing: " + QString::number(num));
        m_number = std::make_shared<NumberDataType>(num);
        emit newData(0);
    }
}
