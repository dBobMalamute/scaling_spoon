#include "InverseCosineIdea.h"
#include "QtMath"
#include <QDebug>

InverseCosineAttributes::InverseCosineAttributes()
{
    m_listName = "Inverse Cosine";
    m_displayName = "Inverse \nCosine";
    m_authors << "dBob";
    m_description = "This idea calculates the inverse cosine (acos) of the input number";
    m_iconPath = "qrc:/Icons/InverseCosine.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;
    m_embeddedQMLPath = ":/QML/RadianDegreeSwitch.qml";
}

InverseCosineIdea::InverseCosineIdea()
{
    m_degreeMode = true;
}

const IdeaAttributes InverseCosineIdea::attributes()
{
    return Magic<InverseCosineAttributes>::cast();
}

QJsonObject InverseCosineIdea::save() const
{
    QJsonObject saveJson;
    saveJson["deg"] = m_degreeMode;
    return saveJson;
}

void InverseCosineIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["deg"];
    if(!v.isUndefined())
    {
        m_degreeMode = v.toBool();
        emit degreeModeChanged(m_degreeMode);
    }
}

void InverseCosineIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> InverseCosineIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

bool InverseCosineIdea::degreeMode() const
{
    return m_degreeMode;
}

void InverseCosineIdea::setDegreeMode(bool degreeMode)
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

void InverseCosineIdea::validate()
{
    auto n = m_input.lock();

    if(n)
    {
        if(n->number() < -1.0 || n->number() > 1.0)
        {
            setStatus(IdeaStatus::Error, "Complex Answer", "The inverse cosine of a number outside of the range -1 to 1 is a complex number");
        }
        else
        {
            QString longMessage = "Calculating the inverse cosine of " + QString::number(n->number());

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
            setStatus(IdeaStatus::Warning, "Multiple Answers", longMessage);
            compute(n->number());
        }
    }
    else
    {
        setDefaultStatus();
    }
}

void InverseCosineIdea::compute(double n)
{
    emit startSpinningGear();
    if(degreeMode())
        m_result = std::make_shared<NumberDataType>(57.29577951*qAcos(n));
    else //Radians.
        m_result = std::make_shared<NumberDataType>(qAsin(n));
    emit newData(0);
    emit stopSpinningGear();
}


