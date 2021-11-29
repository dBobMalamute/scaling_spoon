#include "CeilingIdea.h"

CeilingAttributes::CeilingAttributes()
{
    m_listName = "Ceiling";
    m_displayName = "Ceiling";
    m_authors << "dBob";
    m_description = "Calculates the whole number above the input number";
    m_iconPath = "qrc:/Icons/Ceiling.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/CeilingIdea.qml";
}

CeilingIdea::CeilingIdea()
{

}

const IdeaAttributes CeilingIdea::attributes()
{
    return Magic<CeilingAttributes>::cast();
}

void CeilingIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> CeilingIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void CeilingIdea::validate()
{
    auto n = m_input.lock();
    if(n)
    {
        setStatus(IdeaStatus::Working, "Calculating", "Calculating the ceiling of " + QString::number(n->number()));
        compute(n->number());
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void CeilingIdea::compute(double n)
{
    emit startSpinningGear();
    int answer = static_cast<int>(n >= 0 ? n+0.9999999999 : n - 0.000000001);
    emit displayNumbers(n, answer);

    m_result = std::make_shared<NumberDataType>(answer);
    emit newData(0);
    emit stopSpinningGear();
}

