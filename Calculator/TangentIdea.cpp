#include "TangentIdea.h"
#include "QtMath"
#include <QDebug>

TangentAttributes::TangentAttributes()
{
    m_listName = "Tangent";
    m_displayName = "Tangent";
    m_authors << "dBob";
    m_description = "This idea calculates the tangent of the input number";
    m_iconPath = "qrc:/Icons/Tangent.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/TangentIdea.qml";

    m_gear = true;
}

TangentIdea::TangentIdea()
{
    m_degreeMode = true;
}

const IdeaAttributes TangentIdea::attributes()
{
    return Magic<TangentAttributes>::cast();
}

QJsonObject TangentIdea::save() const
{
    QJsonObject saveJson;
    saveJson["deg"] = m_degreeMode;
    return saveJson;
}

void TangentIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["deg"];
    if(!v.isUndefined())
    {
        m_degreeMode = v.toBool();
        emit degreeModeChanged(m_degreeMode);
    }
}

void TangentIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> TangentIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

bool TangentIdea::degreeMode() const
{
    return m_degreeMode;
}

void TangentIdea::setDegreeMode(bool degreeMode)
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

void TangentIdea::validate()
{
    auto n = m_input.lock();

    if(n)
    {
        //Note that qTan operates in radian mode.
        // Get number between 0 and 2PI. in radian mode.
        double mod;
        if(degreeMode())
        {
            mod = fmod(n->number(),360);
            mod = qDegreesToRadians(mod);
        }
        else
        {
            mod = fmod(n->number(), 2.0*M_PI);
        }

        if(qFuzzyCompare(mod,M_PI_2) || qFuzzyCompare(mod, 3.0* M_PI_2) ||
                qFuzzyCompare(mod, -1.0* M_PI_2) || qFuzzyCompare(mod, -3.0* M_PI_2))
        {
            setStatus(IdeaStatus::Error, "Undefined", "The tangent of the input is undefined. This is not allowed");
            emit clearNumbers();

        }
        else
        {
            QString longMessage = "Calculating the Tangent of " + QString::number(n->number());
            if(degreeMode())
                longMessage.append(" in degrees");
            else
                longMessage.append(" in radians");
            setStatus(IdeaStatus::Working, "Calculating", longMessage);
            compute(mod);
        }
    }
    else
    {
        emit clearNumbers();
        setDefaultStatus();
    }
}

void TangentIdea::compute(double n)
{
    emit startSpinningGear();

    double tan;

    if(qFuzzyIsNull(n))
        tan = 0.0;
    else if (qFuzzyCompare(n, M_PI))
        tan = 0.0;
    else if (qFuzzyCompare(n, -1.0*M_PI))
        tan = 0.0;
    else
        tan = qTan(n);
    emit displayNumbers(n, tan);

    m_result = std::make_shared<NumberDataType>(tan);
    emit newData(0);
    emit stopSpinningGear();
}


