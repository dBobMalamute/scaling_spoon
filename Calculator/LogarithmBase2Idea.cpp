#include "LogarithmBase2Idea.h"
#include <QtMath>

LogarithmBase2Attributes::LogarithmBase2Attributes()
{
    m_listName = "Logarithm Base 2";
    m_displayName = "Logarithm Base 2";
    m_authors << "dBob";
    m_description = "Calculates the logarithm base 2 of the inputted number";
    m_iconPath = "qrc:/Icons/LogarithmBase2.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/LogarithmBase2Idea.qml";
    m_minimizedQMLPath = ":/QML/LogarithmBase2Minimized.qml";
    m_minimizable = true;
    m_minimizedName = "Logarithm\nBase 2";
    m_differentNameOnMinimized = true;

    m_gear = true;
}


LogarithmBase2Idea::LogarithmBase2Idea()
{

}

const IdeaAttributes LogarithmBase2Idea::attributes()
{
    return Magic<LogarithmBase2Attributes>::cast();
}

void LogarithmBase2Idea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> LogarithmBase2Idea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void LogarithmBase2Idea::validate()
{
    auto n1 = m_input.lock();
    if(n1)
    {
        if(n1->number() > 0.0)
        {
            setStatus(IdeaStatus::Working, "Logging", "Currently calcuating the log base 2 of " + QString::number(n1->number()));
            compute(n1->number());
        }
        else
        {
            setStatus(IdeaStatus::Error, "Imaginary Answer", "The logarithm of a non-positive number is an imaginary number. Imaginary numbers are not currently supported");
            emit clearNumbers();
        }
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void LogarithmBase2Idea::compute(double n1)
{
    emit startSpinningGear();

    double log = qLn(n1) / qLn(2);
    emit displayNumbers(n1, log);

    m_result = std::make_shared<NumberDataType>(log);
    emit newData(0);
    emit stopSpinningGear();
}

void LogarithmBase2Idea::resetDisplay()
{
    validate();
}
