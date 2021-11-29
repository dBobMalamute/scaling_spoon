#include "BinomialDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

BinomialDistributionAttributes::BinomialDistributionAttributes()
{
    m_listName = "Binomial Distribution";
    m_displayName = "Binomial Distribution";
    m_authors << "dBob";
    m_description = "This box creates a binomial distribution. This distribution represents the number of times an event"
                  " will likely occur given it's probability and the number of chances.";
    m_iconPath = "qrc:/Icons/BinomialDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Num" << "Prob";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/BinomialDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Binomial\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes BinomialDistributionIdea::attributes()
{
    return Magic<BinomialDistributionAttributes>::cast();
}

void BinomialDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

BinomialDistributionIdea::BinomialDistributionIdea()
{
    m_options << true << true;
}

void BinomialDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto input = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_numberInput = input;
    else
        m_probInput = input;

    validate();
}

std::shared_ptr<DataType> BinomialDistributionIdea::dataOut(PlugNumber plugNumber)
{
    Q_UNUSED(plugNumber)
    return std::static_pointer_cast<DataType>(m_distribution);
}

void BinomialDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> BinomialDistributionIdea::options() const
{
    return m_options;
}

void BinomialDistributionIdea::setBound(double value, int index)
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

void BinomialDistributionIdea::setAutoFit(bool autoFit)
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

void BinomialDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void BinomialDistributionIdea::validate()
{
    auto v_number = m_numberInput.lock();
    auto v_prob = m_probInput.lock();

    if(v_number && v_prob)
    {
        if(v_number->number() <= 0.0)
        {
            setStatus(IdeaStatus::Error, "Num <= 0", "The input number " + QString::number(v_number->number()) +
                      " is less than 0. This is not a valid input.");
            m_display->clearData();
        }
        else if (v_prob->number() <= 0.0)
        {
            setStatus(IdeaStatus::Error, "Prob <= 0",  "The input probability " + QString::number(v_prob->number())
                      + " is less than 0. This is not a valid probability.");
            m_display->clearData();
        }
        else if (v_prob->number() >= 1.0)
        {
            setStatus(IdeaStatus::Error, "Prob >= 1", "The input probability " + QString::number(v_prob->number()) +
                      " is greater than 1. This is not a valid probability.");
            m_display->clearData();
        }
        else
        {
            int num = static_cast<int>(v_number->number() + 0.5);
            QString longMessage = "Currently creating a binomial distribution for " + QString::number(num) +
                    " event, and a probability of: " + QString::number(v_prob->number());
            setStatus(IdeaStatus::Working, "Calculating a Binomial", longMessage);

            compute(num, v_prob->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void BinomialDistributionIdea::compute(int number, double prob)
{
    emit startSpinningGear();

    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(number);
    yValues.reserve(number);


    //Prepare helpful terms.
    double nFactorial = 1;
    for (int i = 2; i < number + 1; i++)
        nFactorial *= i;
    double q = 1.0 - prob;
    double kFactorial = 1;
    double nMinusKFactorial = nFactorial;
    double pToK = 1.0;
    double qToNMinusK = qPow(q, number);

    //The loop.
    for (int i = 0; i < number; i++)
    {
        xValues.push_back(1.0*i);
        yValues.push_back(nFactorial / kFactorial / nMinusKFactorial * pToK * qToNMinusK);

        kFactorial *= (i + 1);
        nMinusKFactorial /= (number - i);
        pToK *= prob;
        qToNMinusK /= q;
    }

    //Avoid div by 0 in last iteration of loop.
    xValues.push_back(1.0 * number);
    yValues.push_back(pToK);

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Discrete, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Discrete);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject BinomialDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 2; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void BinomialDistributionIdea::load(const QJsonObject &obj)
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
