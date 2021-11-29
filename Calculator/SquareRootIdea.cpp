#include "SquareRootIdea.h"
#include <QtMath>
#include <QDebug>

SquareRootAttributes::SquareRootAttributes()
{
    m_listName = "Square Root";
    m_displayName = "Square Root";
    m_authors << "dBob";
    m_description = "Calculates the square roots of a number. A square root of a number is the number "
                  "which when multiplied by itself produces the original number";

    m_iconPath = "qrc:/Icons/SquareRoot.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id()
                 << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/SquareRootIdea.qml";
}

SquareRootIdea::SquareRootIdea()
{

}

const IdeaAttributes SquareRootIdea::attributes()
{
    return Magic<SquareRootAttributes>::cast();
}

void SquareRootIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;
    validate();
}

std::shared_ptr<DataType> SquareRootIdea::dataOut(PlugNumber plugNumber)
{
    if(plugNumber == 0)
        return std::static_pointer_cast<DataType>(m_posResult);
    else
        return std::static_pointer_cast<DataType>(m_negResult);
}

void SquareRootIdea::validate()
{
    auto n1 = m_input.lock();
    if(n1)
    {
        if(n1->number() < 0.0)
        {
            setStatus(IdeaStatus::Error, "Imaginary Answer","The Square root of a negative number is an imaginary number. This box assumes you don't want imaginary answers.");
            emit clearNumbers();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating" , "Calcuating the square roots of: " + QString::number(n1->number()));
            compute(n1->number());
        }
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void SquareRootIdea::compute(double n1)
{
    emit startSpinningGear();
    double sqrt = qSqrt(n1);
    double negSqrt = sqrt * -1.0;
    if(sqrt < 0.0)
    {
        emit displayNumbers(n1, negSqrt);
        m_posResult = std::make_shared<NumberDataType>(negSqrt);
        m_negResult = std::make_shared<NumberDataType>(sqrt);
    }
    else
    {
        emit displayNumbers(n1, sqrt);
        m_posResult = std::make_shared<NumberDataType>(sqrt);
        m_negResult = std::make_shared<NumberDataType>(negSqrt);
    }
    emit newDataAllPlugs();
    emit stopSpinningGear();
}

