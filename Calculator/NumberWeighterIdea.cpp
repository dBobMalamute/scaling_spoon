#include "NumberWeighterIdea.h"

NumberWeighterAttributes::NumberWeighterAttributes()
{
    m_listName = "Number Weighter";
    m_displayName = "Number Weighter";
    m_authors << "dBob";
    m_description = "This idea represents addition of two decimal-type numbers.";
    m_iconPath = "qrc:/Icons/NumberWeighter.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/NumberWeighterIdea.qml";

    m_gear = true;
}

NumberWeighterIdea::NumberWeighterIdea()
{
    m_weight = 0.5;
}

const IdeaAttributes NumberWeighterIdea::attributes()
{
    return Magic<NumberWeighterAttributes>::cast();
}

void NumberWeighterIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_number1 = number;
    else
        m_number2 = number;

    validate();
}

std::shared_ptr<DataType> NumberWeighterIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

QJsonObject NumberWeighterIdea::save() const
{
    QJsonObject ideaJson = Idea::save();
    ideaJson["wt"] = m_weight;

    return ideaJson;

}

void NumberWeighterIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["wt"];

    if(!v.isUndefined())
    {
        m_weight = v.toDouble();
        emit weightChanged();
        validate();
    }
}

double NumberWeighterIdea::weight() const
{
    return m_weight;
}

void NumberWeighterIdea::setWeight(double weight)
{
    if (qFuzzyCompare(m_weight, weight))
        return;
    else
    {
        QJsonObject oldParams = this->save();
        m_weight = weight;
        emit weightChanged();

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);
        validate();
    }
}

QString NumberWeighterIdea::topWeightString() const
{
    return QString::number(m_weight,'f',3);
}

QString NumberWeighterIdea::botWeightString() const
{
    return QString::number(1.0-m_weight,'f',3);
}

void NumberWeighterIdea::validate()
{
    auto n1 = m_number1.lock();
    auto n2 = m_number2.lock();

    if(n1 && n2)
    {
        setStatus(IdeaStatus::Working, "Weighting", "Weighting " + QString::number(n1->number()) + " + " + QString::number(n2->number()));
        compute(weight(), n1->number(),n2->number());
    }
    else
    {
        setDefaultStatus();
    }
}

void NumberWeighterIdea::compute(double weight, double n1, double n2)
{
    emit startSpinningGear();
    m_result = std::make_shared<NumberDataType>(weight*n1+(1.0 - weight)*n2);
    emit newData(0);
    emit stopSpinningGear();
}


