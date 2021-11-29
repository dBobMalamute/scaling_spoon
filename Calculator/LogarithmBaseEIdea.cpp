#include "LogarithmBaseEIdea.h"
#include <QtMath>

LogarithmBaseEAttributes::LogarithmBaseEAttributes()
{
    m_listName = "Natural Logarithm";
    m_displayName = "Natural Logarithm";
    m_authors << "dBob";
    m_description = "Calculates the logarithm base e of the inputted number. This is frequently called the natural logarithm (ln).";
    m_iconPath = "qrc:/Icons/LogarithmBaseE.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/LogarithmBaseEIdea.qml";
    m_minimizedQMLPath = ":/QML/LogarithmBaseEMinimized.qml";
    m_minimizable = true;
    m_minimizedName = "Natural\nLogarithm";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

LogarithmBaseEIdea::LogarithmBaseEIdea()
{

}

const IdeaAttributes LogarithmBaseEIdea::attributes()
{
    return Magic<LogarithmBaseEAttributes>::cast();
}

void LogarithmBaseEIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> LogarithmBaseEIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void LogarithmBaseEIdea::validate()
{
    auto n1 = m_input.lock();
    if(n1)
    {
        if(n1->number() > 0.0)
        {
            setStatus(IdeaStatus::Working, "Logging", "Currently calcuating the log base e of " + QString::number(n1->number()));

            compute(n1->number());
        }
        else
        {
            setStatus(IdeaStatus::Error, "Imaginary Answer", "The logarithm of a non-positive number is an imaginary number. "
                                                             "Imaginary numbers are not currently supported");
            emit clearNumbers();
        }
    }
    else
    {
        setDefaultStatus();
        emit clearNumbers();
    }
}

void LogarithmBaseEIdea::compute(double n1)
{
    emit startSpinningGear();

    double log = qLn(n1);
    emit displayNumbers(n1, log);

    m_result = std::make_shared<NumberDataType>(qLn(n1));
    emit newData(0);
    emit stopSpinningGear();
}

void LogarithmBaseEIdea::resetDisplay()
{
    validate();
}

