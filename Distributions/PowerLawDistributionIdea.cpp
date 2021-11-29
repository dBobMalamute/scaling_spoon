#include "PowerLawDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

PowerLawDistributionAttributes::PowerLawDistributionAttributes()
{
    m_listName = "Power Law Distribution";
    m_displayName = "Power Law Distribution";
    m_authors << "dBob";
    m_description = "Description";
    m_iconPath = "qrc:/Icons/PowerLawDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Decay" << "Low";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/PowerLawDistribution.qml";
    m_minimizable = true;
    m_minimizedName = "Power Law\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes PowerLawDistributionIdea::attributes()
{
    return Magic<PowerLawDistributionAttributes>::cast();
}

PowerLawDistributionIdea::PowerLawDistributionIdea()
{
    m_bounds << -5.0 << 5.0 << 0.0 << 1.0;
    m_options << true << true << true << true;
}

void PowerLawDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    if(plugNumber == 0)
        m_decayInput = number;
    else
        m_lowInput = number;

    validate();
}

std::shared_ptr<DataType> PowerLawDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void PowerLawDistributionIdea::resetDisplay()
{
    if(status() == IdeaStatus::Working)
        emit plot(m_xValues, m_yValues);
}

QList<double> PowerLawDistributionIdea::bounds() const
{
    return m_bounds;
}

QList<bool> PowerLawDistributionIdea::options() const
{
    return m_options;
}

double PowerLawDistributionIdea::decay() const
{
    return m_decay;
}

double PowerLawDistributionIdea::low() const
{
    return m_low;
}

void PowerLawDistributionIdea::setBounds(QList<double> bounds)
{
    if (m_bounds == bounds)
        return;

    QJsonObject oldParams = Idea::save();
    QJsonArray oldBoundsArray;
    for(int i = 0; i < 4; i++)
        oldBoundsArray.append(m_bounds.at(i));
    oldParams["b"] = oldBoundsArray;

    m_bounds = bounds;
    emit boundsChanged(m_bounds);

    QJsonObject newParams = Idea::save();
    QJsonArray newBoundsArray;
    for(int i = 0; i < 4; i++)
        newBoundsArray.append(m_bounds.at(i));
    newParams["b"] = newBoundsArray;

    emit reportParamsChanged(oldParams, newParams);
}

void PowerLawDistributionIdea::setBound(double bound, int index)
{
    if(m_bounds[index] == bound)
        return;

    QJsonObject oldParams = save();

    m_bounds[index] = bound;
    emit boundsChanged(m_bounds);

    m_options[0] = false;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);

    if(status() == IdeaStatus::Working)
    {
        if(m_distribution)
            emit plot(m_distribution.get()->xValues(), m_distribution.get()->yValues());
    }
}

void PowerLawDistributionIdea::setOptions(QList<bool> options)
{
    if (m_options == options)
        return;

    QJsonObject oldParams = Idea::save();
    QJsonArray oldOptionsArray;
    for(int i = 0; i < 4; i++)
        oldOptionsArray.append(m_options.at(i));
    oldParams["o"] = oldOptionsArray;

    m_options = options;
    emit optionsChanged(m_options);

    QJsonObject newParams = Idea::save();
    QJsonArray newOptionsArray;
    for(int i = 0; i < 4; i++)
        newOptionsArray.append(m_options.at(i));
    newParams["o"] = newOptionsArray;

    emit reportParamsChanged(oldParams, newParams);
}

void PowerLawDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    if(index == 0 && option == true)
    {
        fitBounds();
    }

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void PowerLawDistributionIdea::validate()
{
    auto v_decay = m_decayInput.lock();
    auto v_low = m_lowInput.lock();

    if(v_decay && v_low)
    {
        if(v_low->number() <= 0.0)
        {
            setStatus(IdeaStatus::Error, "Low <= 0", "The low " + QString::number(v_low->number()) +
                      " is less than 0. ");
            emit plot(std::vector<double>(), std::vector<double>());
        }
        else if(v_decay->number() <= 1.0)
        {
            setStatus(IdeaStatus::Error, "decay <= 1", "The decay " + QString::number(v_low->number()) +
                      " is less than 1.");
            emit plot(std::vector<double>(), std::vector<double>());
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating a Gaussian", "Currently creating a Gaussian distribution with a mean of "
                      + QString::number(v_decay->number()) + " and a standard deviation of " + QString::number(v_low->number()));

            setDecay(v_decay->number());
            setLow(v_low->number());
            compute();
        }
    }
    else
    {
        setDefaultStatus();
        emit plot(std::vector<double>(), std::vector<double>());
    }
}

void PowerLawDistributionIdea::compute()
{
    emit startSpinningGear();
    m_xValues.clear();
    m_yValues.clear();

    m_xValues.reserve(201);
    m_yValues.reserve(201);

    double decayMinus1OverLow = (decay() - 1) / low();

    m_xValues.push_back(low() - 1e-10);
    m_yValues.push_back(0.0);

    double x = low();
    for(int i = 0 ; i < 99; i++)
    {
        m_xValues.push_back(x);
        m_yValues.push_back(decayMinus1OverLow * qPow(x/low(), -1.0*decay()));

        x = low() * qPow( qPow(x/low(),-1.0*decay()) - 0.01*decayMinus1OverLow*low()/(decay() - 1), -1.0/decay());
    }
    for(int i = 0 ; i < 99; i++)
    {
        m_xValues.push_back(x);
        m_yValues.push_back(decayMinus1OverLow * qPow(x/low(), -1.0*decay()));

        x = low() * qPow( qPow(x/low(),-1.0*decay()) - 0.0001*decayMinus1OverLow*low()/(decay() - 1), -1.0/decay());
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, m_xValues,m_yValues);
    emit newData(0);

    emit plot(m_xValues, m_yValues);


    emit stopSpinningGear();
}

void PowerLawDistributionIdea::fitBounds()
{
//    m_bounds.clear();
//    m_bounds << m_mean - 10.0*m_stdev;
//    m_bounds << m_mean + 10.0*m_stdev;
//    m_bounds << 0.0;
//    m_bounds << 1.2 / m_stdev * 0.4; // 1.2* height of gaussian. plug in x = mean. approximate
//    emit boundsChanged(m_bounds);

//    //Redraw the distribution, mean, stdev if it's working.
//    if(status() == IdeaStatus::Working)
//    {
//        if(m_distribution)
//            emit plot(m_distribution.get()->xValues(), m_distribution.get()->yValues());
//    }
}

void PowerLawDistributionIdea::setDecay(double decay)
{
    if (qFuzzyCompare(m_decay, decay))
        return;

    m_decay = decay;
    emit decayChanged(m_decay);
}

void PowerLawDistributionIdea::setLow(double low)
{
    if (qFuzzyCompare(m_low, low))
        return;

    m_low = low;
    emit lowChanged(m_low);
}

QJsonObject PowerLawDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();

    QJsonArray boundsArray;
    for(int i = 0; i < 4; i++)
        boundsArray.append(m_bounds.at(i));
    saveJson["b"] = boundsArray;

    QJsonArray optionsArray;
    for(int i = 0; i < 4; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void PowerLawDistributionIdea::load(const QJsonObject &obj)
{
    QJsonValue v;

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

    v = obj["b"];
    if(!v.isUndefined())
    {
        QJsonArray boundsArray = v.toArray();
        m_bounds.clear();
        for(int i = 0; i < 4; i++)
        {
            m_bounds.append(boundsArray.at(i).toDouble());
        }
        emit boundsChanged(m_bounds);
    }

    validate();
}
