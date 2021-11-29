#include "FloorIdea.h"

FloorAttributes::FloorAttributes()
{
    m_listName = "Floor";
    m_displayName = "Floor";
    m_authors << "dBob";
    m_description = "Calculates the whole number below this number";
    m_iconPath = "qrc:/Icons/Floor.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/FloorIdea.qml";
}


FloorIdea::FloorIdea()
{

}

const IdeaAttributes FloorIdea::attributes()
{
    return Magic<FloorAttributes>::cast();
}

void FloorIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> FloorIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void FloorIdea::validate()
{
    auto n = m_input.lock();

    if(n)
    {
        setStatus(IdeaStatus::Working, "Flooring", "Calculating the floor of " + QString::number(n->number()));
        compute(n->number());
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void FloorIdea::compute(double n)
{
    emit startSpinningGear();
    int answer = static_cast<int>(n >= 0 ? n : n-1);

    emit displayNumbers(n, answer);

    m_result = std::make_shared<NumberDataType>(answer);

    emit newData(0);
    emit stopSpinningGear();
}

