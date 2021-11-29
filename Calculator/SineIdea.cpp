#include "SineIdea.h"
#include "QtMath"
#include <QDebug>


SineAttributes::SineAttributes()
{
    m_listName = "Sine";
    m_displayName = "Sine";
    m_authors << "dBob";
    m_description = "This idea calculates the sine of the input number";
    m_iconPath = "qrc:/Icons/Sine.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/SineIdea.qml";

    m_gear = true;
}

SineIdea::SineIdea()
{
    m_degreeMode = true;
}

const IdeaAttributes SineIdea::attributes()
{
    return Magic<SineAttributes>::cast();
}

QJsonObject SineIdea::save() const
{
    QJsonObject saveJson;
    saveJson["deg"] = m_degreeMode;
    return saveJson;
}

void SineIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["deg"];
    if(!v.isUndefined())
    {
        m_degreeMode = v.toBool();
        emit degreeModeChanged(m_degreeMode);

        validate();
    }
}

void SineIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> SineIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

bool SineIdea::degreeMode() const
{
    return m_degreeMode;
}

void SineIdea::setDegreeMode(bool degreeMode)
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

void SineIdea::validate()
{
    auto n = m_input.lock();

    if(n)
    {
        QString longMessage = "Calculating the Sine of " + QString::number(n->number());
        if(degreeMode())
            longMessage.append(" in degrees");
        else
            longMessage.append(" in radians");

        setStatus(IdeaStatus::Working, "Calculating",longMessage);
        compute(n->number());
    }
    else
    {
        emit clearNumbers();
        setDefaultStatus();
    }
}

void SineIdea::compute(double n)
{
    emit startSpinningGear();

    //Note that qSin operates in radian mode.
    double mod;
    double sin;
    if(degreeMode())
    {
        mod = fmod(n,360);
        mod = qDegreesToRadians(mod);
    }
    else
    {
        mod = fmod(n, 2.0*M_PI);
    }

    if(qFuzzyIsNull(mod))
        sin = 0.0;
    else if (qFuzzyCompare(mod, M_PI))
        sin = 0.0;
    else if (qFuzzyCompare(mod, -1.0*M_PI))
        sin = 0.0;
    else
        sin = qSin(mod);
    emit displayNumbers(n, sin);

    m_result = std::make_shared<NumberDataType>(sin);
    emit newData(0);
    emit stopSpinningGear();
}
