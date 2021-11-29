#include "LogNormalDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

LogNormalDistributionAttributes::LogNormalDistributionAttributes()
{
    m_listName = "Lognormal Distribution";
    m_displayName = "Lognormal Distribution";
    m_authors << "dBob";
    m_description = "This box creates a log normal distribution. The log normal distribution is usually used "
                  "when you have limited information. It is the best distribution to use when you have a rough idea"
                  " of the mean and a percentage guess of variance about the mean, and don't want to claim anything else.";
    m_iconPath = "qrc:/Icons/LognormalDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Mean" << "Stdev";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/LogNormalDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Lognormal\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes LogNormalDistributionIdea::attributes()
{
    return Magic<LogNormalDistributionAttributes>::cast();
}

void LogNormalDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

LogNormalDistributionIdea::LogNormalDistributionIdea()
{
    m_options << true << true << true << true;
}

void LogNormalDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_meanInput = number;
    else
        m_stdevInput = number;

    validate();
}

std::shared_ptr<DataType> LogNormalDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void LogNormalDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> LogNormalDistributionIdea::options() const
{
    return m_options;
}

double LogNormalDistributionIdea::mean() const
{
    return m_mean;
}

double LogNormalDistributionIdea::stdev() const
{
    return m_stdev;
}

void LogNormalDistributionIdea::setBound(double value, int index)
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

void LogNormalDistributionIdea::setAutoFit(bool autoFit)
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

void LogNormalDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void LogNormalDistributionIdea::setMean(double mean)
{
    if (qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void LogNormalDistributionIdea::setStdev(double stdev)
{
    if (qFuzzyCompare(m_stdev, stdev))
        return;

    m_stdev = stdev;
    emit stdevChanged(m_stdev);
}

void LogNormalDistributionIdea::validate()
{
    auto v_mean = m_meanInput.lock();
    auto v_stdev = m_stdevInput.lock();

    if(v_mean && v_stdev)
    {
        if(v_stdev->number() <= 0.0)
        {
            QString longMessage = "The input standard deviation " + QString::number(v_stdev->number()) +
                    " is less than 0. This is not defined";
            setStatus(IdeaStatus::Error, "Stdev <= 0", longMessage);
            m_display->clearData();
        }
        else if(v_mean->number() <= 0)
        {
            QString longMessage = "The input mean of " + QString::number(v_mean->number()) +
                " is less than 0. This is not defined";
            setStatus(IdeaStatus::Error, "Mean <= 0", longMessage);
            m_display->clearData();
        }
        else
        {
            QString longMessage = "Currently creating a Lognormal distribution with a mean of "  + QString::number(v_mean->number()) +
                    " and a standard deviation of " + QString::number(v_stdev->number());
            setStatus(IdeaStatus::Working, "Calculating", longMessage);
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

void LogNormalDistributionIdea::compute(double mean, double stdev)
{
    emit startSpinningGear();
    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(300);
    yValues.reserve(300);

    double mu = qLn(mean*mean/ qSqrt(mean*mean + stdev*stdev));
    double sigma = qSqrt(qLn(1 + stdev*stdev/mean/mean));

    for(int i = -150; i< 151; i++)
    {
        double x_val = mean + stdev * i / 20.0;
        if(x_val < 0)
            continue;

        double interm_val = qLn(x_val) - mu;
        double y_val;
        if(x_val <= 0.0)
            y_val = 0.0;
        else
            y_val = 1.0/x_val/sigma * 0.39894228 *qExp(-1.0*interm_val* interm_val/2.0/sigma/sigma);
        xValues.push_back(x_val);
        yValues.push_back(y_val);
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    m_distribution->setMean(mean);
    m_distribution->setVariance(stdev*stdev);

    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject LogNormalDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 4; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void LogNormalDistributionIdea::load(const QJsonObject &obj)
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
