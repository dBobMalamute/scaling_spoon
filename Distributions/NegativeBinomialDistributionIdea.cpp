#include "NegativeBinomialDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

NegativeBinomialDistributionAttributes::NegativeBinomialDistributionAttributes()
{
    m_listName = "Negative Binomial Distribution";
    m_displayName = "Negative Binomial Distribution";
    m_authors << "dBob";
    m_description = "This box creates a negative binomial distribution. This models the number of failures in"
                  " a sequence of trials with a given success probability before we achieve a certain number of"
                  " successes.";
    m_iconPath = "qrc:/Icons/NegativeBinomialDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Num" << "Prob";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/NegativeBinomialDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Negative\nBinomial\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes NegativeBinomialDistributionIdea::attributes()
{
    return Magic<NegativeBinomialDistributionAttributes>::cast();
}

void NegativeBinomialDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}


NegativeBinomialDistributionIdea::NegativeBinomialDistributionIdea()
{
    m_options << true << true;
}

void NegativeBinomialDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto input = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_numberInput = input;
    else
        m_probInput = input;

    validate();
}

std::shared_ptr<DataType> NegativeBinomialDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void NegativeBinomialDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> NegativeBinomialDistributionIdea::options() const
{
    return m_options;
}

void NegativeBinomialDistributionIdea::setBound(double value, int index)
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

void NegativeBinomialDistributionIdea::setAutoFit(bool autoFit)
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

void NegativeBinomialDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void NegativeBinomialDistributionIdea::validate()
{
    auto v_number = m_numberInput.lock();
    auto v_prob = m_probInput.lock();

    if(v_number && v_prob)
    {
        if(v_number->number() <= 0.0)
        {
            QString longMessage = "The input number " + QString::number(v_number->number()) + " is less than 0. This is not a valid"
                                                                                          "input.";
            setStatus(IdeaStatus::Error, "Num <= 0", longMessage);
            m_display->clearData();
        }
        else if (v_prob->number() <= 0.0)
        {
            QString longMessage = "The input probability " + QString::number(v_prob->number()) + " is less than 0. This is not a valid"
                                                                                          "probability.";
            setStatus(IdeaStatus::Error, "Prob <= 0", longMessage);
            m_display->clearData();
        }
        else if (v_prob->number() >= 1.0)
        {
            QString longMessage = "The input probability " + QString::number(v_prob->number()) + " is greater than 1. This is not a valid"
                                                                                                 "probability.";
            setStatus(IdeaStatus::Error, "Prob >= 1", longMessage);
            m_display->clearData();
        }
        else
        {
            int num = static_cast<int>(v_number->number() + 0.5);
            QString longMessage = "Currently creating a negative binomial distribution for " + QString::number(num) +
                    " event, and a probability of: " + QString::number(v_prob->number());
            setStatus(IdeaStatus::Working, "Calculating", longMessage);
            compute(num, v_prob->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void NegativeBinomialDistributionIdea::compute(int number, double prob)
{
    emit startSpinningGear();

    // Go from 0 to mean + 5 standard deviations + 5.
    // Handles the cases I tested. -dBob
    int max = static_cast<int>(prob * number / (1.0 - prob) + 5.0 * prob*number / (1.0 - prob) / (1.0 - prob) + 5);

    std::vector<double> xValues;
    std::vector<double> yValues;

    double p = qPow(1.0- prob, number);

    for (int k = 0; k < max; k++)
    {
        xValues.push_back(1.0*k);
        yValues.push_back(p);

        p *= prob;
        p *= (k + number);
        p /= (k + 1);
        if( p < 1e-10)
            break;
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Discrete, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Discrete);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject NegativeBinomialDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 2; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void NegativeBinomialDistributionIdea::load(const QJsonObject &obj)
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
