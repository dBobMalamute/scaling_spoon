#include "PowerIdea.h"
#include <QtMath>

PowerAttributes::PowerAttributes()
{
    m_listName = "Power";
    m_displayName = "Power";
    m_authors << "dBob";
    m_description = "Calculates takes the first input to the second input power";
    m_iconPath = "qrc:/Icons/Power.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "base" << "pow";
    m_customLabels = true;

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsOut << "#";

    m_embeddedQMLPath = ":/QML/PowerIdea.qml";
    m_minimizedQMLPath = ":/QML/PowerMinimized.qml";
    m_minimizable = true;

    m_gear = true;
}


PowerIdea::PowerIdea()
{

}

const IdeaAttributes PowerIdea::attributes()
{
    return Magic<PowerAttributes>::cast();
}

void PowerIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);

    if(plugNumber == 0)
        m_base = number;
    else
        m_pow = number;

    validate();
}

std::shared_ptr<DataType> PowerIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void PowerIdea::validate()
{
    auto n1 = m_base.lock();
    auto n2 = m_pow.lock();

    if(n1 && n2)
    {
        setStatus(IdeaStatus::Working,"Calculating","Calculating " + QString::number(n1->number()) +
                  " to the " + QString::number(n2->number()) + "th power");
        compute(n1->number(),n2->number());
    }
    else
    {
        emit clearNumbers();
        setDefaultStatus();
    }
}

void PowerIdea::compute(double n1, double n2)
{
    emit startSpinningGear();

    double pow = qPow(n1,n2);
    emit displayNumbers(n1,n2,pow);

    m_result = std::make_shared<NumberDataType>(pow);
    emit newData(0);
    emit stopSpinningGear();
}

void PowerIdea::resetDisplay()
{
    validate();
}
