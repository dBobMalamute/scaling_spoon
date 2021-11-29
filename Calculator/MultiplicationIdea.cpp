#include "MultiplicationIdea.h"

MultiplicationAttributes::MultiplicationAttributes()
{
    m_listName = "Multiplication";
    m_displayName = "Multiplication";
    m_authors << "dBob";
    m_description = "This idea represents multiplication of two decimal-type numbers.";
    m_iconPath = "qrc:/Icons/Multiplication.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_labelsIn << "" << "";
    m_labelsOut << "";
    m_customLabels = true;

    m_gear = true;

    m_embeddedQMLPath = ":/QML/MultiplicationIdea.qml";
}

MultiplicationIdea::MultiplicationIdea()
{

}

const IdeaAttributes MultiplicationIdea::attributes()
{
    return Magic<MultiplicationAttributes>::cast();
}

void MultiplicationIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_number1 = number;
    else
        m_number2 = number;

    validate();
}

std::shared_ptr<DataType> MultiplicationIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void MultiplicationIdea::validate()
{
    auto n1 = m_number1.lock();
    auto n2 = m_number2.lock();
    if(n1 && n2)
    {
        setStatus(IdeaStatus::Working, "Multiplying", "Multiplying " + QString::number(n1->number()) + " * " + QString::number(n2->number()));
        compute(n1->number(),n2->number());
        emit displayNumbers(n1->number(), n2->number(), n1->number()*n2->number());
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void MultiplicationIdea::compute(double n1, double n2)
{
    emit startSpinningGear();
    m_result = std::make_shared<NumberDataType>(n1*n2);
    emit newData(0);
    emit stopSpinningGear();
}

