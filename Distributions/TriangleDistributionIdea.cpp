#include "TriangleDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

TriangleDistributionAttributes::TriangleDistributionAttributes()
{
    m_listName = "Triangle Distribution";
    m_displayName = "Triangle Distribution";
    m_authors << "dBob";
    m_description = "This box creates a triangle distribution. I don't know why you'd want to use this. If someone knows,"
                  " please let me know. -dBob";

    m_iconPath = "qrc:/Icons/TriangleDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Low" << "Mid" << "Up";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/TriangleDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Triangle\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes TriangleDistributionIdea::attributes()
{
    return Magic<TriangleDistributionAttributes>::cast();
}

void TriangleDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

TriangleDistributionIdea::TriangleDistributionIdea()
{
    m_options << true << true << true << true << true;
}

void TriangleDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_lowInput = number;
    else if(plugNumber == 1)
        m_midInput = number;
    else
        m_upInput = number;

    validate();
}

std::shared_ptr<DataType> TriangleDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void TriangleDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> TriangleDistributionIdea::options() const
{
    return m_options;
}

double TriangleDistributionIdea::low() const
{
    return m_low;
}

double TriangleDistributionIdea::mid() const
{
    return m_mid;
}

double TriangleDistributionIdea::up() const
{
    return m_up;
}

void TriangleDistributionIdea::setBound(double bound, int index)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setBound(bound, index);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);
}

void TriangleDistributionIdea::setAutoFit(bool autoFit)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setAutoFit(autoFit);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);
}


void TriangleDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void TriangleDistributionIdea::setLow(double low)
{
    if (qFuzzyCompare(m_low, low))
        return;

    m_low = low;
    emit lowChanged(m_low);
}

void TriangleDistributionIdea::setMid(double mid)
{
    if (qFuzzyCompare(m_mid, mid))
        return;

    m_mid = mid;
    emit midChanged(m_mid);
}

void TriangleDistributionIdea::setUp(double up)
{
    if (qFuzzyCompare(m_up, up))
        return;

    m_up = up;
    emit upChanged(m_up);
}

void TriangleDistributionIdea::validate()
{
    auto v_low = m_lowInput.lock();
    auto v_mid = m_midInput.lock();
    auto v_up = m_upInput.lock();

    if(v_low && v_mid && v_up)
    {
        if(v_low->number() >= v_up->number())
        {
            QString longMessage = "The inputted lower bound: " + QString::number(v_low->number()) + " is greater than "
                    + "the inputted upper bound: " + QString::number(v_up->number());
            setStatus(IdeaStatus::Error, "low >= up", longMessage);
            m_display->clearData();
        }
        else if(v_mid->number() >= v_up->number())
        {
            QString longMessage = "The inputted middle: " + QString::number(v_low->number()) + " is greater than "
                    + "the inputted upper bound: " + QString::number(v_up->number());
            setStatus(IdeaStatus::Error, "mid >= up", longMessage);
            m_display->clearData();
        }
        else if(v_low->number() >= v_mid->number())
        {
            QString longMessage = "The inputted lower bound: " + QString::number(v_low->number()) + " is greater than "
                    + "the inputted middle: " + QString::number(v_up->number());
            setStatus(IdeaStatus::Error, "low >= mid", longMessage);
            m_display->clearData();
        }
        else
        {
            QString longMessage = "Currently creating a Triangle Distribution with a lower bound of "  + QString::number(v_low->number()) +
                    " a middle of " + QString::number(v_mid->number()) +
                    " and an upper bound of " + QString::number(v_up->number());
            setStatus(IdeaStatus::Working, "Calculating", longMessage);

            setLow(v_low->number());
            setMid(v_mid->number());
            setUp(v_up->number());

            compute(v_low->number(), v_mid->number(), v_up->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void TriangleDistributionIdea::compute(double low, double mid, double up)
{
    emit startSpinningGear();

    double length = up - low;

    std::vector<double> xValues;
    std::vector<double> yValues;

    xValues.reserve(3);
    yValues.reserve(3);
    xValues.push_back(low);
    xValues.push_back(mid);
    xValues.push_back(up);

    yValues.push_back(0.0);
    yValues.push_back(2.0/length);
    yValues.push_back(0.0);

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    m_distribution->setMean((low + mid + up) / 3.0);
    m_distribution->setMode(mid);
    m_distribution->setVariance((low*low + mid*mid + up*up - low*mid - low*up - mid*up) / 18.0);
    m_distribution->setSkew(qSqrt(2.0) *(low + up - 2.0*mid)*(2.0*low - mid - up)*(low -2.0*up + mid) / 5.0
                            / qPow((low*low + mid*mid + up*up - low*mid - low*up - mid*up ),1.5));
    m_distribution->setKurtosis(2.4);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject TriangleDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 5; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void TriangleDistributionIdea::load(const QJsonObject &obj)
{
    QJsonValue v = obj["d"];
    if(!v.isUndefined())
    {
        m_display->load(v.toObject());
    }

    v = obj["o"];
    if(!v.isUndefined())
    {
        QJsonArray optionsArray = v.toArray();
        m_options.clear();
        for(int i = 0; i < 5; i++)
        {
            m_options.append(optionsArray.at(i).toBool());
        }
        emit optionsChanged(m_options);
    }
}
