#include "UniformDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

UniformDistributionAttributes::UniformDistributionAttributes()
{
    m_listName = "Uniform Distribution";
    m_displayName = "Uniform Distribution";
    m_authors << "dBob";
    m_description = "This box creates a uniform distribution. The uniform distribution is usually used "
                    "when you have limited information. It is the best distribution to use when you have a rough idea"
                    " of the lower and upper bounds, and don't want to claim anything else.";
    m_iconPath = "qrc:/Icons/UniformDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Low" << "Up";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/UniformDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Uniform\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes UniformDistributionIdea::attributes()
{
    return Magic<UniformDistributionAttributes>::cast();
}

void UniformDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}


UniformDistributionIdea::UniformDistributionIdea()
{
    m_options << true << true << true << true;
}

void UniformDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_lowInput = number;
    else
        m_upInput = number;

    validate();
}

std::shared_ptr<DataType> UniformDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void UniformDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> UniformDistributionIdea::options() const
{
    return m_options;
}

double UniformDistributionIdea::low() const
{
    return m_low;
}

double UniformDistributionIdea::up() const
{
    return m_up;
}

void UniformDistributionIdea::setBound(double value, int index)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setBound(value, index);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);
}

void UniformDistributionIdea::setAutoFit(bool autoFit)
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

void UniformDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void UniformDistributionIdea::setLow(double low)
{
    if (qFuzzyCompare(m_low, low))
        return;

    m_low = low;
    emit lowChanged(m_low);
}

void UniformDistributionIdea::setUp(double up)
{
    if (qFuzzyCompare(m_up, up))
        return;

    m_up = up;
    emit upChanged(m_up);
}

void UniformDistributionIdea::validate()
{
    auto v_low = m_lowInput.lock();
    auto v_up = m_upInput.lock();

    if(v_low && v_up)
    {
        if(v_low->number() >= v_up->number())
        {
            QString longMessage = "The inputted lower bound: " + QString::number(v_low->number()) + " is greater than "
                    + "the inputted upper bound: " + QString::number(v_up->number());
            setStatus(IdeaStatus::Error, "low >= up", longMessage);
            m_display->clearData();
        }
        else
        {
            QString longMessage = "Currently creating a Uniform Distribution with a lower bound of "  + QString::number(v_low->number()) +
                    " and an upper bound of " + QString::number(v_up->number());
            setLow(v_low->number());
            setUp(v_up->number());
            setStatus(IdeaStatus::Working, "Calculating", longMessage);
            compute(v_low->number(), v_up->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void UniformDistributionIdea::compute(double low, double up)
{
    emit startSpinningGear();

    double length = up - low;

    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(2);
    yValues.reserve(2);

    xValues.push_back(low);
    xValues.push_back(up);

    yValues.push_back(1.0/length);
    yValues.push_back(1.0/length);

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    m_distribution->setMean(0.5 * (low + up));
    m_distribution->setMedian(0.5 * (low + up));
    m_distribution->setVariance(1.0/12.0 * (up - low) * (up - low));
    m_distribution->setSkew(0.0);
    m_distribution->setKurtosis(1.8);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject UniformDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 4; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void UniformDistributionIdea::load(const QJsonObject &obj)
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
        for(int i = 0; i < 4; i++)
        {
            m_options.append(optionsArray.at(i).toBool());
        }
        emit optionsChanged(m_options);
    }
}

