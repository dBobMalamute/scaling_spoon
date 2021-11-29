#include "ReciprocalIdea.h"

ReciprocalAttributes::ReciprocalAttributes()
{
    m_listName = "Reciprocal";
    m_displayName = "Reciprocal";
    m_authors << "dBob";
    m_description = "Calculates the reciprocal of a number.";
    m_iconPath = "qrc:/Icons/Reciprocal.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_labelsIn << "";
    m_labelsOut << "";
    m_customLabels = true;

    m_gear = true;
    m_embeddedQMLPath = ":/QML/ReciprocalIdea.qml";
}

ReciprocalIdea::ReciprocalIdea()
{

}

const IdeaAttributes ReciprocalIdea::attributes()
{
    return Magic<ReciprocalAttributes>::cast();
}

void ReciprocalIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;
    validate();
}

std::shared_ptr<DataType> ReciprocalIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void ReciprocalIdea::validate()
{
    auto n1 = m_input.lock();
    if(n1)
    {
        if(qFuzzyCompare(n1->number(),0))
        {
            setStatus(IdeaStatus::Error, "Division by 0", "The inverse of 0 is not defined. L'hopitals rule may help in this situation");
            emit clearNumbers();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Inversing", "Calculating the inverse of " + QString::number(n1->number()));
            compute(n1->number());
            emit displayNumbers(n1->number(), 1.0 / n1->number());
        }
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void ReciprocalIdea::compute(double n1)
{
    emit startSpinningGear();
    m_result = std::make_shared<NumberDataType>(1.0/n1);
    emit newData(0);
    emit stopSpinningGear();
}


