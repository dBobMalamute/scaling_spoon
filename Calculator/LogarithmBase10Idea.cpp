#include "LogarithmBase10Idea.h"
#include <QtMath>

LogarithmBase10Attributes::LogarithmBase10Attributes()
{
    m_listName = "Logarithm Base 10";
    m_displayName = "Logarithm Base 10";
    m_authors << "dBob";
    m_description = "Calculates the logarithm base 10 of the inputted number";
    m_iconPath = "qrc:/Icons/LogarithmBase10.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/LogarithmBase10Idea.qml";
    m_minimizedQMLPath = ":/QML/LogarithmBase10Minimized.qml";
    m_minimizable = true;
    m_minimizedName = "Logarithm\nBase 10";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

LogarithmBase10Idea::LogarithmBase10Idea()
{

}

const IdeaAttributes LogarithmBase10Idea::attributes()
{
    return Magic<LogarithmBase10Attributes>::cast();
}

void LogarithmBase10Idea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> LogarithmBase10Idea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void LogarithmBase10Idea::validate()
{
    auto n = m_input.lock();
    if(n)
    {
        if(n->number() > 0.0)
        {
            setStatus(IdeaStatus::Working, "Logging", "Currently calcuating the log base 10 of " + QString::number(n->number()));
            compute(n->number());
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

void LogarithmBase10Idea::compute(double n)
{
    emit startSpinningGear();

    double log = qLn(n) / qLn(10);
    emit displayNumbers(n, log);

    m_result = std::make_shared<NumberDataType>(log);
    emit newData(0);
    emit stopSpinningGear();
}


void LogarithmBase10Idea::resetDisplay()
{
    validate();
}

