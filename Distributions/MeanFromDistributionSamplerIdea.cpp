#include "MeanFromDistributionSamplerIdea.h"
#include <QJsonArray>
#include <QRandomGenerator>
#include <QtMath>

#include "DistributionCommonFunctions.h"

MeanFromDistributionSamplerAttributes::MeanFromDistributionSamplerAttributes()
{
    m_listName = "Mean from Distribution Sampler";
    m_displayName = "Mean From Distribution Sampler";
    m_authors << "dBob";
    m_description = "This box takes in a distribution and samples number from it. It features a nice visual when maximized";
    m_iconPath = "qrc:/Icons/MeanFromDistributionSampler.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumbersDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/MeanFromDistributionSamplerIdea.qml";

    m_minimizable = true;
    m_startMinimized = true;
    m_minimizedQMLPath = ":/QML/MeanFromDistributionSamplerMinimized.qml";
    m_differentNameOnMinimized = true;
    m_minimizedName = "Mean From\nDistribution Sampler";

    m_externalQMLPaths << ":/QML/MeanFromDistributionControls.qml";
    m_externalQMLLocations.append({true, false, 10,0});
}

const IdeaAttributes MeanFromDistributionSamplerIdea::attributes()
{
    return Magic<MeanFromDistributionSamplerAttributes>::cast();
}

void MeanFromDistributionSamplerIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

MeanFromDistributionSamplerIdea::MeanFromDistributionSamplerIdea() :
    m_animationSpeed(2.72)
{
    m_samplingSize = 1;
    m_samplesForGridView.clear();
    m_samplesFromDistribution.clear();
    m_sampleMeans.clear();
    m_totalSamples = 0;
    m_currentSamples = 0;
    m_samplesPerMean = 10;

    m_samplesInImplicitBin.reserve(80);
    for(int i = 0; i < 80; i++)
        m_samplesInImplicitBin.append(0);
}

void MeanFromDistributionSamplerIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto distribution = std::dynamic_pointer_cast<DistributionDataType>(dataType);
    m_distribution = distribution;

    validate();
}

QJsonObject MeanFromDistributionSamplerIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    saveJson["s"] = m_samplingSize;
    saveJson["n"] = m_totalSamples;
    saveJson["sm"] = m_samplesPerMean;
    saveJson["as"] = m_animationSpeed;

    return saveJson;
}

void MeanFromDistributionSamplerIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["d"];
    if(!v.isUndefined())
    {
        m_display->load(v.toObject());
    }

    v = p["s"];
    if(!v.isUndefined())
    {
        m_samplingSize = v.toInt();
        emit samplingSizeChanged(m_samplingSize);
    }

    v = p["n"];
    if(!v.isUndefined())
    {
        m_totalSamples = v.toInt();
        emit totalSamplesChanged(m_totalSamples);
        resetSamples();
        sample();
    }

    v = p["sm"];
    if(!v.isUndefined())
    {
        m_samplesPerMean = v.toInt();
        emit samplesPerMeanChanged(m_totalSamples);
        // Probably need to reset.
    }

    v = p["as"];
    if(!v.isUndefined())
    {
        m_animationSpeed = v.toDouble();
        emit animationSpeedChanged(m_animationSpeed);
    }
}

void MeanFromDistributionSamplerIdea::resetDisplay()
{
    for(int i = 0; i < 80; i++)
        m_samplesInImplicitBin[i] = 0;

    if(valid())
        m_display->plot();

    emit resetAnimation();
    emit startAnimation();
    emit samplesChanged(m_samplesForGridView);
}

QList<QVariant> MeanFromDistributionSamplerIdea::samples() const
{
    return m_samplesForGridView;
}

int MeanFromDistributionSamplerIdea::samplingSize() const
{
    return m_samplingSize;
}

int MeanFromDistributionSamplerIdea::samplesPerMean() const
{
    return m_samplesPerMean;
}

int MeanFromDistributionSamplerIdea::totalSamples() const
{
    return m_totalSamples;
}

double MeanFromDistributionSamplerIdea::animationSpeed() const
{
    return m_animationSpeed;
}

void MeanFromDistributionSamplerIdea::validate()
{
    auto v_distribution = m_distribution.lock();
    if(v_distribution)
    {
        setStatus(IdeaStatus::Working, "Plotting", "This idea box is currently plotting the input distribution.");
        m_distributionType = v_distribution->distType();
        m_xValues = v_distribution->xValues();
        m_yValues = v_distribution->yValues();

        m_cdf = DistributionCommonFunctions::makeCDF(m_distributionType, m_xValues, m_yValues);
        m_display->setXValues(m_xValues);
        m_display->setYValues(m_yValues);
        m_display->setDistType(m_distributionType);
        m_display->plot();

        resetSamples();
        sample();
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
        resetSamples();
    }
}

void MeanFromDistributionSamplerIdea::setBound(double bound, int index)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setBound(bound, index);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);
}

void MeanFromDistributionSamplerIdea::setAutoFit(bool autoFit)
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
void MeanFromDistributionSamplerIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

void MeanFromDistributionSamplerIdea::setTotalSamples(int totalSamples)
{
    if (m_totalSamples == totalSamples)
        return;

    QJsonObject oldParams = Idea::save();
    oldParams["n"] = m_totalSamples;

    m_totalSamples = totalSamples;
    emit totalSamplesChanged(m_totalSamples);

    QJsonObject newParams = Idea::save();
    newParams["n"] = m_totalSamples;

    emit reportParamsChanged(oldParams, newParams);

    sample();
}

void MeanFromDistributionSamplerIdea::setSamples(QList<QVariant> samples)
{
    if (m_samplesForGridView == samples)
        return;

    m_samplesForGridView = samples;
    emit samplesChanged(m_samplesForGridView);
}

void MeanFromDistributionSamplerIdea::setSamplingSize(int samplingSize)
{
    if (m_samplingSize == samplingSize)
        return;

    QJsonObject oldParams = Idea::save();
    oldParams["s"] = m_samplingSize;

    m_samplingSize = samplingSize;
    emit samplingSizeChanged(m_samplingSize);

    QJsonObject newParams = Idea::save();
    newParams["s"] = m_samplingSize;
    emit reportParamsChanged(oldParams, newParams);
}

void MeanFromDistributionSamplerIdea::setSamplesPerMean(int samplesPerMean)
{
    if (m_samplesPerMean == samplesPerMean)
        return;

    resetSamples();

    QJsonObject oldParams = Idea::save();
    oldParams["sm"] = m_samplesPerMean;

    m_samplesPerMean = samplesPerMean;
    emit samplesPerMeanChanged(m_samplesPerMean);

    QJsonObject newParams = Idea::save();
    newParams["sm"] = m_samplesPerMean;
    emit reportParamsChanged(oldParams, newParams);

    sample();
}

void MeanFromDistributionSamplerIdea::sampleButtonPressed()
{
    setTotalSamples(m_totalSamples + m_samplingSize);
}

void MeanFromDistributionSamplerIdea::resampleButtonPressed()
{
    resetSamples();
    sample();
}

void MeanFromDistributionSamplerIdea::totalSamplesTextEdited(int samples)
{
    if(m_totalSamples == samples)
        return;

    resetSamples();
    setTotalSamples(samples);
}

void MeanFromDistributionSamplerIdea::sample()
{
    // This isn't the smartest thing in the world. -dBob.
    if(status() != IdeaStatus::Working)
        return;

    if(m_totalSamples == 0)
    {
        m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleMeans);
        emit newData(0);
        return;
    }
    emit startSpinningGear();

    if(m_distribution.lock()->distType() == DistributionDataType::Continuous)
    {
        while(m_currentSamples < m_totalSamples)
        {
            double mean = 0.0;
            m_samplesFromDistribution.append(QList<double>());
            m_samplesFromDistribution[m_currentSamples].reserve(m_samplesPerMean);
            for(int i = 0; i < m_samplesPerMean; i++)
            {
                double random = QRandomGenerator::global()->generateDouble();
                auto lower = std::lower_bound(m_cdf.begin(),m_cdf.end(), random);

                int index = std::distance(m_cdf.begin(), lower);

                //Change in y over change in x
                double deltaY_deltaX = (m_yValues[index] - m_yValues[index - 1]) / (m_xValues[index] - m_xValues[index - 1]);

                //2 formulas for area. Area = random - m_cdf[index-1]
                // Area = 0.5*(x- x[index-1])*(y[index-1] + y[index-1] *deltay/deltax *(x - x[index-1]);
                // Set areas equal. Use quadratic formula. Always positive answer.
                double xVal;
                if(!qFuzzyIsNull(deltaY_deltaX))
                {
                   xVal = m_xValues[index - 1] + (-1.0*m_yValues[index - 1] + qSqrt(m_yValues[index - 1] * m_yValues[index - 1]
                           + 2 * deltaY_deltaX * (random - m_cdf[index - 1]))) / (deltaY_deltaX);
                }
                else
                {
                    xVal = m_xValues[index - 1] + (random - m_cdf[index - 1]) / m_yValues[index-1];
                }

                mean += xVal;
                m_samplesFromDistribution[m_currentSamples].append(xVal);
            }
            mean /= m_samplesPerMean;

            m_samplesForGridView.append(mean);
            m_sampleMeans.append(mean);
            m_currentSamples++;
        }
    }
    else // Discrete
    {
        while(m_currentSamples < m_totalSamples)
        {
            double mean = 0.0;
            m_samplesFromDistribution.append(QList<double>());
            m_samplesFromDistribution[m_currentSamples].reserve(m_samplesPerMean);
            for(int i = 0; i < m_samplesPerMean; i++)
            {
                double random = QRandomGenerator::global()->generateDouble();
                auto lower = std::lower_bound(m_cdf.begin(),m_cdf.end(), random);

                int index = std::distance(m_cdf.begin(), lower);

                double xVal = m_xValues[index];

                mean += xVal;
                m_samplesFromDistribution[m_currentSamples].append(xVal);
            }
            mean /= m_samplesPerMean;

            m_samplesForGridView.append(mean);
            m_sampleMeans.append(mean);
            m_currentSamples++;
        }
    }

    emit startAnimation();

    emit samplesChanged(m_samplesForGridView);

    m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleMeans);
    emit newData(0);
    emit stopSpinningGear();
}

QPointF MeanFromDistributionSamplerIdea::getPositionForSampleDot(int i)
{
    //450 is the width. Position is percentage.
    if(m_sampleMeans.isEmpty())
        return QPointF(0,60000);

    double xValue = 450 * (m_sampleMeans[i] - m_display->xLow())/
            (m_display->xHigh() - m_display->xLow());
    int implicitBinIndex = qFloor((80 * xValue / 450));
    double yValue = 290 - 2.0 * m_samplesInImplicitBin.at(implicitBinIndex);
    m_samplesInImplicitBin[implicitBinIndex]++;

    return QPointF(xValue, yValue);
}

QList<double> MeanFromDistributionSamplerIdea::getPositionsForMeanDots(int i)
{
    QList<double> positions;
    positions.reserve(m_samplesPerMean);

    if(m_samplesFromDistribution.isEmpty())
    {
        QList<double> nullList;
        nullList.reserve(m_samplesPerMean);
        for(int j = 0; j < m_samplesPerMean; j++)
            nullList.append(-1.0);
        return nullList;
    }

    QList<double> rawValues = m_samplesFromDistribution[i];
    double length = m_display->xHigh() - m_display->xLow();

    for(int j = 0; j < m_samplesPerMean; j++)
    {
        double xValue = 450 * (rawValues[j] - m_display->xLow()) / length;
        positions.append(xValue);
    }

    return positions;
}

void MeanFromDistributionSamplerIdea::resetSamples()
{
    m_currentSamples = 0;

    for(int i = 0; i < 80; i++)
        m_samplesInImplicitBin[i] = 0;

    m_samplesForGridView.clear();
    emit samplesChanged(m_samplesForGridView);
    m_sampleMeans.clear();
    m_samplesFromDistribution.clear();

    emit resetAnimation();

    m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleMeans);
    emit newData(0);
}

std::shared_ptr<DataType> MeanFromDistributionSamplerIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_samplesPointer);
}
