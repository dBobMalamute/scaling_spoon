#include "InverseSineIdea.h"
#include "QtMath"
#include <QDebug>

InverseSineAttributes::InverseSineAttributes()
{
    m_listName = "Inverse Sine";
    m_displayName = "Inverse\nSine";
    m_authors << "dBob";
    m_description = "This idea calculates the inverse sine (asin) of the input number";
    m_iconPath = "qrc:/Icons/InverseSine.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/RadianDegreeSwitch.qml";
}

InverseSineIdea::InverseSineIdea()
{
    m_degreeMode = true;
}

const IdeaAttributes InverseSineIdea::attributes()
{
    return Magic<InverseSineAttributes>::cast();
}

QJsonObject InverseSineIdea::save() const
{
    QJsonObject saveJson;
    saveJson["deg"] = m_degreeMode;
    return saveJson;
}

void InverseSineIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["deg"];
    if(!v.isUndefined())
    {
        m_degreeMode = v.toBool();
        emit degreeModeChanged(m_degreeMode);
    }
}

void InverseSineIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> InverseSineIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

bool InverseSineIdea::degreeMode() const
{
    return m_degreeMode;
}

void InverseSineIdea::setDegreeMode(bool degreeMode)
{
    if (m_degreeMode == degreeMode)
        return;

    QJsonObject oldParams = save();
    m_degreeMode = degreeMode;
    emit degreeModeChanged(m_degreeMode);
    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams,newParams);

    validate();
}

void InverseSineIdea::validate()
{
    auto n = m_input.lock();

    if(n)
    {
        if(n->number() < -1.0 || n->number() > 1.0)
        {
            setStatus(IdeaStatus::Error, "Complex Answer", "The inverse sine of a number outside of the range -1 to 1 is a complex number");
        }
        else
        {
            QString longMessage = "Calculating the Inverse Sine of " + QString::number(n->number());
            if(degreeMode())
            {
                longMessage.append(" in degrees");
                longMessage.append("There are infinitely many answers. They are the answer + or - 360 an infinite number of times");

            }
            else
            {
                longMessage.append(" in radians");
                longMessage.append("There are infinitely many answers. They are the answer + or - 2PI an infinite number of times");
            }
            setStatus(IdeaStatus::Warning,"Multiple Answers" ,longMessage);
            compute(n->number());
        }
    }
    else
    {
        setDefaultStatus();
    }
}

void InverseSineIdea::compute(double n)
{
    emit startSpinningGear();
    if(degreeMode())
        m_result = std::make_shared<NumberDataType>(57.29577951*qAsin(n));
    else //Radians.
        m_result = std::make_shared<NumberDataType>(qAsin(n));
    emit newData(0);
    emit stopSpinningGear();
}

