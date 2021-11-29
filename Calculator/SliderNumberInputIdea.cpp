#include "SliderNumberInputIdea.h"

#include <QJsonValue>
#include <QDebug>

SliderNumberInputAttributes::SliderNumberInputAttributes()
{
    m_listName = "Slider Input";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This idea allows a user to use a slider to input a number. The upper and lower limit of the slider"
                    " can be set.";
    m_iconPath = "qrc:/Icons/SliderNumberInput.png";

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/SliderNumberInputIdea.qml";
}

SliderNumberInputIdea::SliderNumberInputIdea()
{
    m_number = 4.20;
    setNumber(4.20);

    m_lowLimit = 0;
    m_upLimit = 10;
    updateOutputNumber();
    setStatus(IdeaStatus::Working, "Sourcing: 4.20", "");
    QObject::connect(this,&SliderNumberInputIdea::numberChanged,this,&SliderNumberInputIdea::updateOutputNumber);
}

const IdeaAttributes SliderNumberInputIdea::attributes()
{
    return Magic<SliderNumberInputAttributes>::cast();
}

QJsonObject SliderNumberInputIdea::save() const
{
    QJsonObject ideaJson = Idea::save();

    ideaJson["up"] = m_upLimit;
    ideaJson["num"] = m_number;
    ideaJson["low"] = m_lowLimit;

    return ideaJson;
}

void SliderNumberInputIdea::load(QJsonObject const &p)
{
    QJsonValue v = p["up"];
    if (!v.isUndefined())
    {
        m_upLimit = v.toDouble();
        emit upLimitChanged();
    }

    v = p["num"];
    if(!v.isUndefined())
    {
        m_number = v.toDouble();
        emit numberChanged();
    }

    v = p["low"];
    if(!v.isUndefined())
    {
        m_lowLimit = v.toDouble();
        emit lowLimitChanged();
    }

    emit newData(0);
}

std::shared_ptr<DataType> SliderNumberInputIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_output);
}

double SliderNumberInputIdea::number()
{
    return dealWithFloatingPointJunk(m_number);
}

double SliderNumberInputIdea::upLimit()
{
    return dealWithFloatingPointJunk(m_upLimit);
}

double SliderNumberInputIdea::lowLimit()
{
    return dealWithFloatingPointJunk(m_lowLimit);
}

double SliderNumberInputIdea::sliderPos() const
{
    return (m_number - m_lowLimit) / (m_upLimit - m_lowLimit);
}

void SliderNumberInputIdea::setNumber(double number)
{
    if(qFuzzyCompare(m_number,number))
    {
        return;
    }
    else if(number > m_upLimit || number < m_lowLimit)
    {
        emit numberChanged();
        return;
    }
    else
    {
        QJsonObject oldParams = this->save();
        m_number = number;
        emit newData(0);
        emit numberChanged();

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);

        setStatus(IdeaStatus::Working, "Sourcing", "Sourcing: " + QString::number(m_number));
    }
}

void SliderNumberInputIdea::setUpLimit(double upLimit)
{
    if (qFuzzyCompare(m_upLimit,upLimit))
        return;
    else if( upLimit < m_lowLimit)
    {
        emit upLimitChanged();
        return;
    }
    else
    {
        QJsonObject oldParams = this->save();
        m_upLimit = upLimit;
        emit upLimitChanged();

        if(m_number > m_upLimit)
        {
            m_number = m_upLimit;
            emit numberChanged();
        }

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams, newParams);
    }
}

void SliderNumberInputIdea::setLowLimit(double lowLimit)
{
    if (qFuzzyCompare(m_lowLimit, lowLimit))
        return;
    else if(lowLimit > m_upLimit)
    {
        emit lowLimitChanged();
        return;
    }
    else
    {
        QJsonObject oldParams = this->save();
        m_lowLimit = lowLimit;
        emit lowLimitChanged();

        if(m_number < m_lowLimit)
        {
            m_number = m_lowLimit;
            emit numberChanged();
        }

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams, newParams);
    }
}

void SliderNumberInputIdea::setNumberBySlider(double value)
{
    m_number = m_lowLimit + value * (m_upLimit - m_lowLimit);
    emit numberChanged();
}

void SliderNumberInputIdea::updateOutputNumber()
{
    m_output = std::make_shared<NumberDataType>(m_number);
    emit newData(0);
}

double SliderNumberInputIdea::dealWithFloatingPointJunk(double input)
{
    if(qFuzzyIsNull(input))
        return 0.0;
    else
        return input;
}
