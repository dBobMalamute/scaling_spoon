#include "PowerOfTwoIdea.h"
#include <QtMath>

PowerOfTwoIdeaAttributes::PowerOfTwoIdeaAttributes()
{
    m_listName = "Power of 2";
    m_displayName = "Power of 2";
    m_authors << "dBob";
    m_description = "This idea calculates 2 to the inputted number.";
    m_iconPath = "qrc:/Icons/PowerOfTwo.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();


    m_embeddedQMLPath = ":/QML/PowerOfTwoIdea.qml";

    m_minimizedQMLPath = ":/QML/PowerOfTwoMinimized.qml";
    m_minimizable = true;

    m_gear = true;
}

PowerOfTwoIdea::PowerOfTwoIdea()
{

}

const IdeaAttributes PowerOfTwoIdea::attributes()
{
    return Magic<PowerOfTwoIdeaAttributes>::cast();
}

void PowerOfTwoIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> PowerOfTwoIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void PowerOfTwoIdea::validate()
{
    auto n1 = m_input.lock();
    if(n1)
    {
        setStatus(IdeaStatus::Working, "Calculating", "Calculating 2 to the " + QString::number(n1->number()) + " power.");
        compute(n1->number());
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void PowerOfTwoIdea::compute(double n1)
{
    emit startSpinningGear();

    double exp = qPow(2.0,n1);

    emit displayNumbers(n1, exp);

    m_result = std::make_shared<NumberDataType>(exp);
    emit newData(0);

    emit stopSpinningGear();
}

void PowerOfTwoIdea::resetDisplay()
{
    validate();
}

