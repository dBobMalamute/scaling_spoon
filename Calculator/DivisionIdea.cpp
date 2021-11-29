#include "DivisionIdea.h"

DivisionAttributes::DivisionAttributes()
{
    m_listName = "Division";
    m_displayName = "Division";
    m_authors << "dBob";
    m_description = "This idea represents the division of one number by another";
    m_iconPath = "qrc:/Icons/Division.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_labelsIn << "" << "";
    m_labelsOut << "";
    m_customLabels = true;

    m_gear = true;

    m_embeddedQMLPath = ":/QML/DivisionIdea.qml";
}

const IdeaAttributes DivisionIdea::attributes()
{
    return Magic<DivisionAttributes>::cast();
}

DivisionIdea::DivisionIdea()
{

}

void DivisionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_number1 = number;
    else
        m_number2 = number;

    validate();
}

std::shared_ptr<DataType> DivisionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void DivisionIdea::validate()
{
    auto n1 = m_number1.lock();
    auto n2 = m_number2.lock();
    if(n1 && n2)
    {
        if(qFuzzyCompare(n2->number(),0))
        {
            setStatus(IdeaStatus::Error, "Division by 0", "Division by 0 is not supported. In many cases where this comes up L'hopitals rule can help.");
            emit clearNumbers();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Dividing", "Dividing " + QString::number(n1->number()) + " / " + QString::number(n2->number()));
            compute(n1->number(),n2->number());
            emit displayNumbers(n1->number(), n2->number(), n1->number() / n2->number());
        }
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void DivisionIdea::compute(double n1, double n2)
{
    emit startSpinningGear();
    m_result = std::make_shared<NumberDataType>(n1/n2);
    emit newData(0);
    emit stopSpinningGear();
}


