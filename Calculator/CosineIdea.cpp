#include "CosineIdea.h"
#include "QtMath"
#include <QDebug>

CosineAttributes::CosineAttributes()
{
    m_listName = "Cosine";
    m_displayName = "Cosine";
    m_authors << "dBob";
    m_description = "This idea calculates the cosine of the input number";
    m_iconPath = "qrc:/Icons/Cosine.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/CosineIdea.qml";
}

CosineIdea::CosineIdea()
{
    m_degreeMode = true;
}

const IdeaAttributes CosineIdea::attributes()
{
    return Magic<CosineAttributes>::cast();
}

QJsonObject CosineIdea::save() const
{
    QJsonObject saveJson;
    saveJson["deg"] = m_degreeMode;

    return saveJson;
}

void CosineIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["deg"];
    if(!v.isUndefined())
    {
        m_degreeMode = v.toBool();
        emit degreeModeChanged(m_degreeMode);

        validate();
    }
}

void CosineIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> CosineIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_result);
}

bool CosineIdea::degreeMode() const
{
    return m_degreeMode;
}

void CosineIdea::setDegreeMode(bool degreeMode)
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

void CosineIdea::validate()
{
    auto n = m_input.lock();

    if(n)
    {
        QString longMessage = "Calculating the Cosine of " + QString::number(n->number());
        if(degreeMode())
            longMessage.append(" in degrees");
        else
            longMessage.append(" in radians");

        setStatus(IdeaStatus::Working, "Calculating", longMessage);
        compute(n->number());
    }
    else
    {
        emit clearNumbers();
        setDefaultStatus();
    }
}

void CosineIdea::compute(double n)
{
    emit startSpinningGear();

    //Note that qCos operates in radian mode.
    double mod;
    double cos;
    if(degreeMode())
    {
        mod = fmod(n,360);
        mod = qDegreesToRadians(mod);
    }
    else
    {
        mod = fmod(n, 2.0*M_PI);
    }

    if(qFuzzyCompare(mod, M_PI_2))
        cos = 0.0;
    else if (qFuzzyCompare(mod, 3*M_PI_2))
        cos = 0.0;
    else if (qFuzzyCompare(mod, -1.0*M_PI_2))
        cos = 0.0;
    else if (qFuzzyCompare(mod, -3.0*M_PI_2))
        cos = 0.0;
    else
        cos = qCos(mod);
    emit displayNumbers(n, cos);

    m_result = std::make_shared<NumberDataType>(cos);
    emit newData(0);
    emit stopSpinningGear();
}
