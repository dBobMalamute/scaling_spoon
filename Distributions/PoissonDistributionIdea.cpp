#include "PoissonDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

PoissonDistributionAttributes::PoissonDistributionAttributes()
{
    m_listName = "Poisson Distribution";
    m_displayName = "Poisson Distribution";
    m_authors << "dBob";
    m_description = "This box creates an Poisson distribution. The common use case of a poisson distribution is if you know"
                    " the average number of times an event happens and you want to generalize that to a distribution. I.e. "
                    " an average of 2 wars occur every decade, what is the distribution of wars I should expect in a typical"
                    " decade.";

    m_iconPath = "qrc:/Icons/PoissonDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Mean";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/PoissonDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Poisson\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes PoissonDistributionIdea::attributes()
{
    return Magic<PoissonDistributionAttributes>::cast();
}

void PoissonDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

PoissonDistributionIdea::PoissonDistributionIdea()
{
    m_options << true << false << true;
}

void PoissonDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_meanInput = number;

    validate();
}

std::shared_ptr<DataType> PoissonDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void PoissonDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> PoissonDistributionIdea::options() const
{
    return m_options;
}

double PoissonDistributionIdea::mean() const
{
    return m_mean;
}

void PoissonDistributionIdea::setBound(double value, int index)
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

void PoissonDistributionIdea::setAutoFit(bool autoFit)
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

void PoissonDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void PoissonDistributionIdea::setMean(double mean)
{
    if (qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void PoissonDistributionIdea::validate()
{
    auto v_mean = m_meanInput.lock();

    if(v_mean)
    {
        if(v_mean->number() <= 0.0)
        {
            QString longMessage = "The input mean " + QString::number(v_mean->number()) +
                    " is less than 0. This is not a valid input.";
            setStatus(IdeaStatus::Error, "Mean <= 0", longMessage);
            m_display->clearData();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating", "Currently creating a poisson distribution with a mean of "
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

void PoissonDistributionIdea::compute(double mean)
{
    emit startSpinningGear();

    int max = qFloor(4.0*mean + 10);
    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(max);
    yValues.reserve(max);

    double running_p_i = qExp(-1.0*mean);

    for (int i = 0; i < max; i++)
    {
        xValues.push_back(1.0*i);
        yValues.push_back(running_p_i);
        running_p_i *= mean/(i+1);
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Discrete,xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Discrete);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject PoissonDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 3; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void PoissonDistributionIdea::load(const QJsonObject &obj)
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
