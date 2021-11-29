#include "SubtractionIdea.h"

SubtractionAttributes::SubtractionAttributes()
{
    m_listName = "Subtraction";
    m_displayName = "Subtraction";
    m_authors << "dBob";
    m_description = "This idea represents subtraction of one decimal type number from another.";
    m_iconPath = "qrc:/Icons/Subtraction.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_labelsIn << "" << "";
    m_labelsOut << "";
    m_customLabels = true;

    m_gear = true;

    m_embeddedQMLPath = ":/QML/SubtractionIdea.qml";
}

SubtractionIdea::SubtractionIdea()
{

}

const IdeaAttributes SubtractionIdea::attributes()
{
    return Magic<SubtractionAttributes>::cast();
}

void SubtractionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_number1 = number;
    else
        m_number2 = number;

    validate();
}

std::shared_ptr<DataType> SubtractionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void SubtractionIdea::validate()
{
    auto n1 = m_number1.lock();
    auto n2 = m_number2.lock();
    if(n1 && n2)
    {
        setStatus(IdeaStatus::Working, "Subtracting", "Subtracting " + QString::number(n1->number()) + " - " + QString::number(n2->number()));
        compute(n1->number(),n2->number());
        emit displayNumbers(n1->number(), n2->number(), n1->number() - n2->number());
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void SubtractionIdea::compute(double n1, double n2)
{
    emit startSpinningGear();
    m_result = std::make_shared<NumberDataType>(n1-n2);
    emit newData(0);
    emit stopSpinningGear();
}


