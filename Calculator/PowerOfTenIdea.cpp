#include "PowerOfTenIdea.h"
#include <QtMath>

PowerOfTenIdeaAttributes::PowerOfTenIdeaAttributes()
{
    m_listName = "Power of 10";
    m_displayName = "Power of 10";
    m_authors << "dBob";
    m_description = "This idea calculates 10 to the inputted number.";
    m_iconPath = "qrc:/Icons/PowerOfTen.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();


    m_embeddedQMLPath = ":/QML/PowerOfTenIdea.qml";

    m_minimizedQMLPath = ":/QML/PowerOfTenMinimized.qml";
    m_minimizable = true;

    m_gear = true;
}

PowerOfTenIdea::PowerOfTenIdea()
{

}

const IdeaAttributes PowerOfTenIdea::attributes()
{
    return Magic<PowerOfTenIdeaAttributes>::cast();
}

void PowerOfTenIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> PowerOfTenIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void PowerOfTenIdea::validate()
{
    auto n1 = m_input.lock();
    if(n1)
    {
        setStatus(IdeaStatus::Working, "Calculating", "Calculating 10 to the " +
                  QString::number(n1->number()) + " power.");
        compute(n1->number());
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void PowerOfTenIdea::compute(double n1)
{
    emit startSpinningGear();

    double exp = qPow(10.0,n1);

    emit displayNumbers(n1, exp);

    m_result = std::make_shared<NumberDataType>(exp);
    emit newData(0);

    emit stopSpinningGear();
}

void PowerOfTenIdea::resetDisplay()
{
    validate();
}

