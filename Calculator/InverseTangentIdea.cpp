#include "InverseTangentIdea.h"
#include "QtMath"
#include <QDebug>

InverseTangentAttributes::InverseTangentAttributes()
{
    m_listName = "Inverse Tangent";
    m_displayName = "Inverse\nTangent";
    m_authors << "dBob";
    m_description = "This idea calculates the inverse tangent (atan) of the input number";
    m_iconPath = "qrc:/Icons/InverseTangent.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/RadianDegreeSwitch.qml";
}

InverseTangentIdea::InverseTangentIdea()
{
    m_degreeMode = true;
}

const IdeaAttributes InverseTangentIdea::attributes()
{
    return Magic<InverseTangentAttributes>::cast();
}

QJsonObject InverseTangentIdea::save() const
{
    QJsonObject saveJson;
    saveJson["deg"] = m_degreeMode;
    return saveJson;
}

void InverseTangentIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["deg"];
    if(!v.isUndefined())
    {
        m_degreeMode = v.toBool();
        emit degreeModeChanged(m_degreeMode);
    }
}

void InverseTangentIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber )
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> InverseTangentIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

bool InverseTangentIdea::degreeMode() const
{
    return m_degreeMode;
}

void InverseTangentIdea::setDegreeMode(bool degreeMode)
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

void InverseTangentIdea::validate()
{
    auto n = m_input.lock();
    if(n)
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
        setStatus(IdeaStatus::Warning, "Multiple Answers", longMessage);
        compute(n->number());
    }
    else
    {
        setDefaultStatus();
    }
}

void InverseTangentIdea::compute(double n)
{
    emit startSpinningGear();
    if(degreeMode())
        m_result = std::make_shared<NumberDataType>(57.29577951*qAtan(n));
    else //Radians.
        m_result = std::make_shared<NumberDataType>(qAsin(n));
    emit newData(0);
    emit stopSpinningGear();
}

