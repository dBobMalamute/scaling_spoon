#include "GeometricDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

GeometricDistributionAttributes::GeometricDistributionAttributes()
{
    m_listName = "Geometric Distribution";
    m_displayName = "Geometric Distribution";
    m_authors << "dBob";
    m_description = "This box creates a geometric distribution. There are two distributions called the geometric distribution..."
                  " This is the one where the trials begin at 1. The other is currently not supported."
                  " This distribution measures the number of attempts to get one success for a given probability.";
    m_iconPath = "qrc:/Icons/GeometricDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Prob";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/GeometricDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Geometric\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes GeometricDistributionIdea::attributes()
{
    return Magic<GeometricDistributionAttributes>::cast();
}

void GeometricDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}


GeometricDistributionIdea::GeometricDistributionIdea()
{
    m_options << true << true;
}

void GeometricDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_probInput = number;

    validate();
}

std::shared_ptr<DataType> GeometricDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void GeometricDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> GeometricDistributionIdea::options() const
{
    return m_options;
}

void GeometricDistributionIdea::setBound(double value, int index)
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

void GeometricDistributionIdea::setAutoFit(bool autoFit)
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

void GeometricDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void GeometricDistributionIdea::validate()
{
    auto v_prob = m_probInput.lock();

    if(v_prob)
    {
        if(v_prob->number() <= 0.0)
        {
            QString longMessage = "The input prob " + QString::number(v_prob->number()) + " is less than 0. This is not a valid"
                                                                                          "probability.";
            setStatus(IdeaStatus::Error,"Prob <= 0" ,longMessage);
            m_display->clearData();
        }
        else if(v_prob->number() >= 1.0)
        {
            QString longMessage = "The input prob " + QString::number(v_prob->number()) + " is greater than 1. This is not a valid"
                                                                                          "probability.";
            setStatus(IdeaStatus::Error,"Prob >= 1" ,longMessage);
            m_display->clearData();
        }

        else
        {
            QString longMessage = "Currently creating a geometric distribution with a probability of "  + QString::number(v_prob->number());
            setStatus(IdeaStatus::Working,"Calculating" ,longMessage);
            compute(v_prob->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void GeometricDistributionIdea::compute(double prob)
{
    emit startSpinningGear();
    int upperLimit = static_cast<int>(qMax(30.0, 10.0/prob));

    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(upperLimit + 1);
    yValues.reserve(upperLimit + 1);

    double running_p_i = 1.0;

    for (int i = 0; i < upperLimit; i++)
    {
        double prob_at_i = prob* running_p_i;
        running_p_i *= (1.0 - prob);

        xValues.push_back(1.0*i);
        yValues.push_back(prob_at_i);
    }


    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Discrete, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Discrete);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject GeometricDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 2; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void GeometricDistributionIdea::load(const QJsonObject &obj)
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
        for(int i = 0; i < 2; i++)
        {
            m_options.append(optionsArray.at(i).toBool());
        }
        emit optionsChanged(m_options);
    }
}
