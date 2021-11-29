#include "BetaDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

BetaDistributionAttributes::BetaDistributionAttributes()
{
    m_listName = "Beta Distribution";
    m_displayName = "Beta Distribution";
    m_authors << "dBob";
    m_description = "This box creates a Beta distribution. This distribution comes about a lot in"
                  " behavior of random variables. It's also a congugate prior of itself which makes things"
                  " easier for people who use formulas.";
    m_iconPath = "qrc:/Icons/BetaDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Alpha" << "Beta";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/BetaDistributionIdea.qml";
    m_minimizable = true;
    m_differentNameOnMinimized = true;
    m_minimizedName = "Beta\nDistribution";

    m_gear = true;
}

const IdeaAttributes BetaDistributionIdea::attributes()
{
    return Magic<BetaDistributionAttributes>::cast();
}

BetaDistributionIdea::BetaDistributionIdea()
{
    m_options << true << true << true << true;
}

void BetaDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

void BetaDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_alphaInput = number;
    else
        m_betaInput = number;

    validate();
}

std::shared_ptr<DataType> BetaDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void BetaDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> BetaDistributionIdea::options() const
{
    return m_options;
}

double BetaDistributionIdea::alpha() const
{
    return m_alpha;
}

double BetaDistributionIdea::beta() const
{
    return m_beta;
}

void BetaDistributionIdea::setBound(double value, int index)
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

void BetaDistributionIdea::setAutoFit(bool autoFit)
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

void BetaDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void BetaDistributionIdea::setAlpha(double alpha)
{
    if (qFuzzyCompare(m_alpha, alpha))
        return;

    m_alpha = alpha;
    emit alphaChanged(m_alpha);
}

void BetaDistributionIdea::setBeta(double beta)
{
    if (qFuzzyCompare(m_beta, beta))
        return;

    m_beta = beta;
    emit betaChanged(m_beta);
}

void BetaDistributionIdea::validate()
{
    auto v_alpha = m_alphaInput.lock();
    auto v_beta = m_betaInput.lock();

    if(v_alpha && v_beta)
    {
        if(v_alpha->number() <= 0.0)
        {
            setStatus(IdeaStatus::Error, "Alpha <= 0", "The input alpha " + QString::number(v_alpha->number()) + " is less than 0. This is undefined");
            m_display->clearData();

        }
        else if(v_beta->number() <= 0.0)
        {
            setStatus(IdeaStatus::Error, "Beta <= 0", "The input beta " + QString::number(v_beta->number()) + " is less than 0. This is undefined");
            m_display->clearData();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating a Beta Distribution", "Currently creating a Beta distribution with an alpha of "  + QString::number(v_alpha->number()) +
                      " and a beta of " + QString::number(v_beta->number()));
            setAlpha(v_alpha->number());
            setBeta(v_beta->number());
            compute(v_alpha->number(), v_beta->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void BetaDistributionIdea::compute(double alpha, double beta)
{
    emit startSpinningGear();

    std::vector<double> xValues;
    std::vector<double> yValues;
    std::vector<double> preYValues;

    xValues.reserve(122);
    preYValues.reserve(120);
    yValues.reserve(122);

    for (int i = 0; i <= 240; i++) //Creates a non-normalized distribution.
    {
        double x = i / 240.0 ;
        double y = qPow(x,alpha - 1) * qPow(1.0-x,beta - 1);
        xValues.push_back(x);
        preYValues.push_back(y);
    }

    double normalizationConstant = 0.0;
    for (int i = 0; i <= 240; i++) //Calculates the normalization constant.
    {
        normalizationConstant += preYValues[i];
    }
    normalizationConstant /= 241.0;

    for (int i = 0; i <= 240; i++) //Creates the normalized Distribution.
    {
        double y = preYValues[i] / normalizationConstant;
        yValues.push_back(y);
    }


    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    m_distribution->setMean(alpha / (alpha + beta));
    //m_distribution->setMedian(mean); TODO regularized incomplete beta function calculation here.
    if(alpha > 1 && beta > 1)
        m_distribution->setMode((alpha - 1) / (alpha + beta -2));
    m_distribution->setVariance((alpha * beta) / ((alpha + beta)*(alpha + beta)*(alpha + beta + 1)));
    m_distribution->setSkew(2 * (beta - alpha) * qSqrt(alpha + beta + 1) / (alpha + beta + 2) / qSqrt(alpha * beta));
    //m_distribution->setKurtosis(3.0 + ); TODO add kurtosis calculation.
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject BetaDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 4; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void BetaDistributionIdea::load(const QJsonObject &obj)
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
