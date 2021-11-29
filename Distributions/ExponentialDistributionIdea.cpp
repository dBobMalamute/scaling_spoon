#include "ExponentialDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

ExponentialDistributionAttributes::ExponentialDistributionAttributes()
{
    m_listName = "Exponential Distribution";
    m_displayName = "Exponential Distribution";
    m_authors << "dBob";
    m_description = "This box creates an Exponential distribution. The exponential distribution is usually used "
                    "when you have limited information. It is the best distribution to use when you have a rough idea"
                    " of the mean, and don't want to claim anything else.";

    m_iconPath = "qrc:/Icons/ExponentialDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Mean";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/ExponentialDistributionIdea.qml";

    m_gear = true;

    m_minimizable = true;
    m_differentNameOnMinimized = true;
    m_minimizedName = "Exponential\nDistribution";
}

const IdeaAttributes ExponentialDistributionIdea::attributes()
{
    return Magic<ExponentialDistributionAttributes>::cast();
}

void ExponentialDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

ExponentialDistributionIdea::ExponentialDistributionIdea()
{
    m_options << true << true << true;
}


void ExponentialDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    Q_UNUSED(plugNumber)
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_meanInput = number;

    validate();
}

std::shared_ptr<DataType> ExponentialDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void ExponentialDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> ExponentialDistributionIdea::options() const
{
    return m_options;
}

double ExponentialDistributionIdea::mean() const
{
    return m_mean;
}

void ExponentialDistributionIdea::setBound(double value, int index)
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

void ExponentialDistributionIdea::setAutoFit(bool autoFit)
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

void ExponentialDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void ExponentialDistributionIdea::setMean(double mean)
{
    if (qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void ExponentialDistributionIdea::validate()
{
    auto v_mean = m_meanInput.lock();

    if(v_mean)
    {
        if(v_mean->number() <= 0.0)
        {
            setStatus(IdeaStatus::Error, "Mean <= 0", "The input mean " + QString::number(v_mean->number()) +
                      " is less than 0. This is not a valid input.");
            m_display->clearData();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating", "Currently creating an exponential distribution with a mean of "
                      + QString::number(v_mean->number()));
            setMean(v_mean->number());
            compute(v_mean->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void ExponentialDistributionIdea::compute(double mean)
{
    emit startSpinningGear();

    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(302);
    yValues.reserve(302);

    // Calculate the Normalization first.
    double normalization = mean / 40.0;
    for (int i = 1; i < 301; i++)
    {
        normalization += mean/20.0*qExp(-1.0*(i)/20.0);
    }

    for (int i = 0; i < 301; i++)
    {
        xValues.push_back((mean*(i)/20.0));
        yValues.push_back(qExp(-1.0*(i)/20.0)/normalization);
    }
    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues,yValues);
    m_distribution->setMean(mean);
    m_distribution->setVariance(mean * mean);
    m_distribution->setMedian(qLn(2) * mean);
    m_distribution->setSkew(2);
    m_distribution->setKurtosis(9);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject ExponentialDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 3; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void ExponentialDistributionIdea::load(const QJsonObject &obj)
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
        for(int i = 0; i < 3; i++)
        {
            m_options.append(optionsArray.at(i).toBool());
        }
        emit optionsChanged(m_options);
    }
}

