#include "ExponentialIdea.h"
#include <QtMath>

ExponentialAttributes::ExponentialAttributes()
{
    m_listName = "Exponential";
    m_displayName = "Exponential";
    m_authors << "dBob";
    m_description = "This idea calculates e (2.721828) to the inputted number.";
    m_iconPath = "qrc:/Icons/Exponential.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/ExponentialIdea.qml";

    m_minimizedQMLPath = ":/QML/ExponentialMinimized.qml";
    m_minimizable = true;
}

ExponentialIdea::ExponentialIdea()
{

}

const IdeaAttributes ExponentialIdea::attributes()
{
    return Magic<ExponentialAttributes>::cast();
}

void ExponentialIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> ExponentialIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void ExponentialIdea::validate()
{
    auto n1 = m_input.lock();
    if(n1)
    {
        setStatus(IdeaStatus::Working, "Exponentiating", "Calculating e to the " + QString::number(n1->number()) + " power.");
        compute(n1->number());
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void ExponentialIdea::compute(double n1)
{
    emit startSpinningGear();

    double exp = qExp(n1);

    emit displayNumbers(n1, exp);

    m_result = std::make_shared<NumberDataType>(exp);
    emit newData(0);

    emit stopSpinningGear();
}

void ExponentialIdea::resetDisplay()
{
    validate();
}
