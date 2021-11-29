#include "SquaredIdea.h"
#include <QDebug>

SquaredAttributes::SquaredAttributes()
{
    m_listName = "Square";
    m_displayName = "Square";
    m_authors << "dBob";
    m_description = "Calcuates the square of a number. The number multiplied by itself.";
    m_iconPath = "qrc:/Icons/Squared.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/SquaredIdea.qml";
}

SquaredIdea::SquaredIdea()
{

}

const IdeaAttributes SquaredIdea::attributes()
{
    return Magic<SquaredAttributes>::cast();
}

void SquaredIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;
    validate();
}

std::shared_ptr<DataType> SquaredIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void SquaredIdea::validate()
{
    auto n1 = m_input.lock();
    if(n1)
    {
        setStatus(IdeaStatus::Working, "Squaring", "Squaring: " + QString::number(n1->number()));
        compute(n1->number());
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void SquaredIdea::compute(double n1)
{
    emit startSpinningGear();
    double sqr = n1*n1;
    emit displayNumbers(n1, sqr);

    m_result = std::make_shared<NumberDataType>(sqr);
    emit newData(0);
    emit stopSpinningGear();
}


