#include "GaussianDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

GaussianDistributionAttributes::GaussianDistributionAttributes()
{
    m_listName = "Gaussian Distribution";
    m_displayName = "Gaussian Distribution";
    m_authors << "dBob";
    m_description = "This box creates a Gaussian distribution. The Gaussian or normal distribution is usually used "
                    "when you have limited information. It is the best distribution to use when you have a rough idea"
                    " of the mean and standard deviation, and don't want to claim anything else.";
    m_iconPath = "qrc:/Icons/GaussianDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Mean" << "Stdev";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/GaussianDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Gaussian\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;

}

const IdeaAttributes GaussianDistributionIdea::attributes()
{
    return Magic<GaussianDistributionAttributes>::cast();
}

void GaussianDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

GaussianDistributionIdea::GaussianDistributionIdea()
{
    m_options << true << true << true << true;
}

void GaussianDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_meanInput = number;
    else
        m_stdevInput = number;

    validate();
}

std::shared_ptr<DataType> GaussianDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void GaussianDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> GaussianDistributionIdea::options() const
{
    return m_options;
}

double GaussianDistributionIdea::mean() const
{
    return m_mean;
}

double GaussianDistributionIdea::stdev() const
{
    return m_stdev;
}

void GaussianDistributionIdea::setBound(double value, int index)
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

void GaussianDistributionIdea::setAutoFit(bool autoFit)
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

void GaussianDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void GaussianDistributionIdea::setMean(double mean)
{
    if (qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void GaussianDistributionIdea::setStdev(double stdev)
{
    if (qFuzzyCompare(m_stdev, stdev))
        return;

    m_stdev = stdev;
    emit stdevChanged(m_stdev);
}

void GaussianDistributionIdea::validate()
{
    auto v_mean = m_meanInput.lock();
    auto v_stdev = m_stdevInput.lock();

    if(v_mean && v_stdev)
    {
        if(v_stdev->number() <= 0.0)
        {
            setStatus(IdeaStatus::Error, "Stdev <= 0", "The input standard deviation " + QString::number(v_stdev->number()) +
                      " is less than 0. ");
            m_display->clearData();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating a Gaussian", "Currently creating a Gaussian distribution with a mean of "
                      + QString::number(v_mean->number()) + " and a standard deviation of " + QString::number(v_stdev->number()));

            setMean(v_mean->number());
            setStdev(v_stdev->number());
            compute(v_mean->number(), v_stdev->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void GaussianDistributionIdea::compute(double mean, double stdev)
{
    emit startSpinningGear();
    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(240);
    yValues.reserve(240);

    for(int i = -120; i < 121; i++)
    {
        xValues.push_back(mean + stdev * i / 20.0);
        yValues.push_back(qExp(-1.0 * i * i / 800.0) / (qSqrt(2.0* M_PI)*stdev));
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues,yValues);
    m_distribution->setMean(mean);
    m_distribution->setMedian(mean);
    m_distribution->setMode(mean);
    m_distribution->setVariance(stdev*stdev);
    m_distribution->setSkew(0.0);
    m_distribution->setKurtosis(3.0);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject GaussianDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 4; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void GaussianDistributionIdea::load(const QJsonObject &obj)
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
