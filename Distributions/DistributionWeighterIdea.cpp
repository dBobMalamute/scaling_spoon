#include "DistributionWeighterIdea.h"
#include <QDebug>

DistributionWeighterAttributes::DistributionWeighterAttributes()
{
    m_listName = "Distribution Weighter";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This idea allows users to weigh distributions. I.e. I think it should be 60% bins 1, 40% bins 2.";
    m_iconPath = "qrc:/Icons/DistributionWeighter.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id()
                << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsIn << "" << "";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/DistributionWeighterIdea.qml";


    m_gear = true;
}

const IdeaAttributes DistributionWeighterIdea::attributes()
{
    return Magic<DistributionWeighterAttributes>::cast();
}

DistributionWeighterIdea::DistributionWeighterIdea()
{
    m_weight = 0.5;
}

void DistributionWeighterIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto distData = std::dynamic_pointer_cast<DistributionDataType>(dataType);

    if(plugNumber == 0)
        m_inputDist1 = distData;
    else
        m_inputDist2 = distData;

    validate();
}

std::shared_ptr<DataType> DistributionWeighterIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_outputDist);
}

QJsonObject DistributionWeighterIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["w"] = m_weight;

    return saveJson;
}

void DistributionWeighterIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["w"];

    if (!v.isUndefined())
    {
        m_weight = v.toDouble();
        emit weightChanged();
        validate();
    }
}

double DistributionWeighterIdea::weight() const
{
    return m_weight;
}

QString DistributionWeighterIdea::topWeightString() const
{
    return QString::number(m_weight,'f',3);
}

QString DistributionWeighterIdea::botWeightString() const
{
    return QString::number(1.0-m_weight,'f',3);
}

void DistributionWeighterIdea::setWeight(double weight)
{
    if (qFuzzyCompare(m_weight, weight))
        return;
    else
    {
        QJsonObject oldParams = this->save();
        m_weight = weight;
        emit weightChanged();

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);
        validate();
    }
}

void DistributionWeighterIdea::validate()
{
    auto v_dist1 = m_inputDist1.lock();
    auto v_dist2 = m_inputDist2.lock();

    if(v_dist1 && v_dist2)
    {
        if(v_dist1->distType() == DistributionDataType::Continuous && v_dist2->distType() == DistributionDataType::Discrete)
        {
            setStatus(IdeaStatus::Error, "Incompatible Types", "The first distribution is continuous, the second is discrete. "
                                                               "This is not something dBob knows how to handle. If you know, let "
                                                               "him know.");
        }
        else if(v_dist1->distType() == DistributionDataType::Discrete && v_dist2->distType() == DistributionDataType::Continuous)
        {
            setStatus(IdeaStatus::Error, "Incompatible Types", "The first distribution is discrete, the second is continuous. "
                                                               "This is not something dBob knows how to handle. If you know, let "
                                                               "him know.");
        }
        else if(v_dist1->distType() == DistributionDataType::Continuous && v_dist2->distType() == DistributionDataType::Continuous)
        {
            QString longMessage = "Currently creating a new distribution base with a weight of " + QString::number(m_weight) +
                    " on the first distribution and a weight of " + QString::number(1.0 - m_weight) +
                    " on the second distribution.";
            setStatus(IdeaStatus::Working, "Weighting", longMessage);
            computeContinuous();
        }
        else
        {
            //Both discrete.
            QString longMessage = "Currently creating a new distribution base with a weight of " + QString::number(m_weight) +
                    " on the first distribution and a weight of " + QString::number(1.0 - m_weight) +
                    " on the second distribution.";
            setStatus(IdeaStatus::Working, "Weighting", longMessage);
            computeDiscrete();
        }
    }
    else
    {
        setDefaultStatus();
    }
}

void DistributionWeighterIdea::computeContinuous()
{
    emit startSpinningGear();
    std::vector<double> dist1X = m_inputDist1.lock()->xValues();
    std::vector<double> dist1Y = m_inputDist1.lock()->yValues();
    std::vector<double> dist2X = m_inputDist2.lock()->xValues();
    std::vector<double> dist2Y = m_inputDist2.lock()->yValues();

    //Add zero's to the ends of the distribution.
    dist1X.insert(dist1X.begin(), dist1X[0] - 0.000000001);
    dist1Y.insert(dist1Y.begin(), 0);
    dist1X.push_back(dist1X[dist1X.size()-1] + 0.000000001);
    dist1Y.push_back(0);

    //Add zero's to the ends of the distribution.
    dist2X.insert(dist2X.begin(), dist2X[0] - 0.000000001);
    dist2Y.insert(dist2Y.begin(), 0);
    dist2X.push_back(dist2X[dist1X.size()-1] + 0.000000001);
    dist2Y.push_back(0);

    // Create a set of all points. Use a set to avoid duplicates.
    QSet<double> combinedQuadratureSet;
    combinedQuadratureSet.reserve(dist1X.size() + dist2X.size());
    for(size_t i = 0; i < dist1X.size(); i++)
    {
        combinedQuadratureSet.insert(dist1X[i]);
    }
    for(size_t i = 0; i < dist2X.size(); i++)
    {
        combinedQuadratureSet.insert(dist2X[i]);
    }
    std::vector<double> combinedQuadrature(combinedQuadratureSet.begin(), combinedQuadratureSet.end());

//    //Order the quadrature.
    std::sort(combinedQuadrature.begin(),combinedQuadrature.end());
    auto last = std::unique(combinedQuadrature.begin(), combinedQuadrature.end());
    combinedQuadrature.erase(last, combinedQuadrature.end());

//    //Interpolate first distribution to combined quadrature.
    QList<double> dist1_interpolated;
    dist1X.insert(dist1X.begin(), combinedQuadrature[0] - 0.000000001);
    dist1Y.insert(dist1Y.begin(), 0);
    dist1X.push_back(combinedQuadrature[combinedQuadrature.size()-1] + 0.000000001);
    dist1Y.push_back(0);

    int dist1Index = 1;
    double pointAbove = dist1X[1];
    double pointBelow = dist1X[0];
    for(size_t i = 0; i < combinedQuadrature.size(); i++)
    {
        while(pointAbove < combinedQuadrature[i])
        {
            dist1Index++;
            pointAbove = dist1X[dist1Index];
            pointBelow = dist1X[dist1Index-1];
        }

        double yVal;
        if(qAbs(combinedQuadrature[i] - pointAbove) < 0.0000000001)
            yVal = dist1Y[dist1Index];
        else
        {
            yVal = dist1Y[dist1Index]*(combinedQuadrature[i] - pointBelow);
            yVal += dist1Y[dist1Index - 1]*(pointAbove - combinedQuadrature[i]);
            yVal /= (pointAbove - pointBelow);
        }
        dist1_interpolated.append(yVal);
    }

    //Interpolate second distribution to combined quadrature.
    // This step only works if distributions have 0's on the ends.
    QList<double> dist2_interpolated;
    dist2X.insert(dist2X.begin(), combinedQuadrature[0] - 0.000000001);
    dist2Y.insert(dist2Y.begin(), 0);
    dist2X.push_back(combinedQuadrature[combinedQuadrature.size()-1] + 0.000000001);
    dist2Y.push_back(0);

    int dist2Index = 1;
    pointAbove = dist2X[1];
    pointBelow = dist2X[0];
    for(size_t i = 0; i < combinedQuadrature.size(); i++)
    {
        while(pointAbove < combinedQuadrature[i])
        {
            dist2Index++;
            pointAbove = dist2X[dist2Index];
            pointBelow = dist2X[dist2Index-1];
        }

        double yVal;
        if(qAbs(combinedQuadrature[i] - pointAbove) < 0.000000001)
            yVal = dist2Y[dist2Index];
        else
        {
            yVal = dist2Y[dist2Index]*(combinedQuadrature[i] - pointBelow);
            yVal += dist2Y[dist2Index - 1]*(pointAbove - combinedQuadrature[i]);
            yVal /= (pointAbove - pointBelow);
        }
        dist2_interpolated.append(yVal);
    }

    std::vector<double> newYValues;
    for(int i = 0; i < dist1_interpolated.length(); i++)
    {
        newYValues.push_back(dist1_interpolated[i] * m_weight + dist2_interpolated[i] * (1.0 - m_weight));
    }

    m_outputDist = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, combinedQuadrature,newYValues);
    emit newData(0);

    emit stopSpinningGear();
}

void DistributionWeighterIdea::computeDiscrete()
{
    emit startSpinningGear();
    std::vector<double> dist1X = m_inputDist1.lock()->xValues();
    std::vector<double> dist1Y = m_inputDist1.lock()->yValues();
    std::vector<double> dist2X = m_inputDist2.lock()->xValues();
    std::vector<double> dist2Y = m_inputDist2.lock()->yValues();

    QSet<double> combinedQuadratureSet;
    for(size_t i = 0; i < dist1X.size(); i++)
    {
        combinedQuadratureSet.insert(dist1X[i]);
    }
    for(size_t i = 0; i < dist2X.size(); i++)
    {
        combinedQuadratureSet.insert(dist2X[i]);
    }
    std::vector<double> combinedQuadrature(combinedQuadratureSet.begin(), combinedQuadratureSet.end());
    std::sort(combinedQuadrature.begin(),combinedQuadrature.end());

    int dist1Iter = 0;
    int dist2Iter = 0;

    //Append extra large values to the original distributions. Simplifies the loop.
    dist1X.push_back(combinedQuadrature[combinedQuadrature.size() - 1] * 2.0);
    dist2X.push_back(combinedQuadrature[combinedQuadrature.size() - 1] * 2.0);

    std::vector<double> newYValues;
    newYValues.reserve(combinedQuadrature.size());

    for(size_t i = 0; i < combinedQuadrature.size(); i++)
    {
        double value = 0;
        if(qFuzzyCompare(dist1X.at(dist1Iter), combinedQuadrature.at(i)))
        {
            value += m_weight * dist1Y.at(dist1Iter);
            dist1Iter++;
        }
        if(qFuzzyCompare(dist2X.at(dist2Iter), combinedQuadrature.at(i)))
        {
            value += (1.0 - m_weight) * dist2Y.at(dist2Iter);
            dist2Iter++;
        }
        newYValues.push_back(value);
    }

    m_outputDist = std::make_shared<DistributionDataType>(DistributionDataType::Discrete, combinedQuadrature,newYValues);
    emit newData(0);

    emit stopSpinningGear();
}


