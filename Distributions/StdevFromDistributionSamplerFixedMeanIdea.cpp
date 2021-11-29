#include "StdevFromDistributionSamplerFixedMeanIdea.h"
#include <QJsonArray>
#include <QRandomGenerator>
#include <QtMath>

#include "DistributionCommonFunctions.h"

StdevFromDistributionSamplerFixedMeanAttributes::StdevFromDistributionSamplerFixedMeanAttributes()
{
    m_listName = "Standard Deviation from Distribution Sampler with Fixed Mean";
    m_displayName = "Standard Deviation From Distribution Sampler with Fixed Mean";
    m_authors << "dBob";
    m_description = "This box takes in a distribution and samples standard deviations from it. It features a nice visual when maximized";
    m_iconPath = "qrc:/Icons/StandardDeviationFromDistributionFixedMean.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<DistributionDataTypeAttributes>::cast().id();

    m_dataTypesOut << Magic<NumbersDataTypeAttributes>::cast().id();

    m_labelsIn << "Mean" << "Dist";
    m_labelsOut << "Nums";
    m_customLabels = true;

    m_gear = true;

    m_embeddedQMLPath = ":/QML/StdevFromDistributionSamplerIdea.qml";

    m_minimizable = true;
    m_startMinimized = true;
    m_minimizedQMLPath = ":/QML/StdevFromDistributionSamplerMinimized.qml";
    m_differentNameOnMinimized = true;
    m_minimizedName = "Standard Deviation From \nDistribution Sampler Fixed Mean";

    m_externalQMLPaths << ":/QML/StdevFromDistributionControls.qml";
    m_externalQMLLocations.append({true, false, 10,0});
}

const IdeaAttributes StdevFromDistributionSamplerFixedMeanIdea::attributes()
{
    return Magic<StdevFromDistributionSamplerFixedMeanAttributes>::cast();
}

void StdevFromDistributionSamplerFixedMeanIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

StdevFromDistributionSamplerFixedMeanIdea::StdevFromDistributionSamplerFixedMeanIdea() :
    m_animationSpeed(2.72)
{
    m_samplingSize = 1;
    m_samplesForGridView.clear();
    m_samplesFromDistribution.clear();
    m_sampleStdevs.clear();
    m_totalSamples = 0;
    m_currentSamples = 0;
    m_samplesPerStdev = 10;
}

void StdevFromDistributionSamplerFixedMeanIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    if(plugNumber == 1)
    {
        m_distribution = std::dynamic_pointer_cast<DistributionDataType>(dataType);
    }
    else
    {
        m_inputMean = std::dynamic_pointer_cast<NumberDataType>(dataType);
    }

    validate();
}

QJsonObject StdevFromDistributionSamplerFixedMeanIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    saveJson["s"] = m_samplingSize;
    saveJson["n"] = m_totalSamples;
    saveJson["sm"] = m_samplesPerStdev;
    saveJson["as"] = m_animationSpeed;

    return saveJson;
}

void StdevFromDistributionSamplerFixedMeanIdea::load(const QJsonObject &p)
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
        m_samplesPerStdev = v.toInt();
        emit samplesPerStdevChanged(m_totalSamples);
        // Probably need to reset.
    }

    v = p["as"];
    if(!v.isUndefined())
    {
        m_animationSpeed = v.toDouble();
        emit animationSpeedChanged(m_animationSpeed);
    }
}

void StdevFromDistributionSamplerFixedMeanIdea::resetDisplay()
{
    if(valid())
        m_display->plot();

    emit resetAnimation();
    emit startAnimation();
    emit samplesChanged(m_samplesForGridView);
}

QList<QVariant> StdevFromDistributionSamplerFixedMeanIdea::samples() const
{
    return m_samplesForGridView;
}

int StdevFromDistributionSamplerFixedMeanIdea::samplingSize() const
{
    return m_samplingSize;
}

int StdevFromDistributionSamplerFixedMeanIdea::samplesPerStdev() const
{
    return m_samplesPerStdev;
}

int StdevFromDistributionSamplerFixedMeanIdea::totalSamples() const
{
    return m_totalSamples;
}

double StdevFromDistributionSamplerFixedMeanIdea::animationSpeed() const
{
    return m_animationSpeed;
}

double StdevFromDistributionSamplerFixedMeanIdea::mean() const
{
    return m_mean;
}

void StdevFromDistributionSamplerFixedMeanIdea::validate()
{
    auto v_distribution = m_distribution.lock();
    auto v_mean = m_inputMean.lock();

    if(v_distribution && v_mean)
    {
        setStatus(IdeaStatus::Working, "Plotting", "This idea box is currently plotting the input distribution.");
        m_distributionType = v_distribution->distType();
        m_xValues = v_distribution->xValues();
        m_yValues = v_distribution->yValues();

        m_mean = v_mean->number();

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

void StdevFromDistributionSamplerFixedMeanIdea::setBound(double bound, int index)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setBound(bound, index);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);

    resetDisplay();
}

void StdevFromDistributionSamplerFixedMeanIdea::setAutoFit(bool autoFit)
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
void StdevFromDistributionSamplerFixedMeanIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

void StdevFromDistributionSamplerFixedMeanIdea::setMean(double mean)
{
    if (qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void StdevFromDistributionSamplerFixedMeanIdea::setTotalSamples(int totalSamples)
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

void StdevFromDistributionSamplerFixedMeanIdea::setSamples(QList<QVariant> samples)
{
    if (m_samplesForGridView == samples)
        return;

    m_samplesForGridView = samples;
    emit samplesChanged(m_samplesForGridView);
}

void StdevFromDistributionSamplerFixedMeanIdea::setSamplingSize(int samplingSize)
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

void StdevFromDistributionSamplerFixedMeanIdea::setSamplesPerStdev(int samplesPerStdev)
{
    if (m_samplesPerStdev == samplesPerStdev)
        return;

    resetSamples();

    QJsonObject oldParams = Idea::save();
    oldParams["sm"] = m_samplesPerStdev;

    m_samplesPerStdev = samplesPerStdev;
    emit samplesPerStdevChanged(m_samplesPerStdev);

    QJsonObject newParams = Idea::save();
    newParams["sm"] = m_samplesPerStdev;
    emit reportParamsChanged(oldParams, newParams);

    sample();
}

void StdevFromDistributionSamplerFixedMeanIdea::sampleButtonPressed()
{
    setTotalSamples(m_totalSamples + m_samplingSize);
}

void StdevFromDistributionSamplerFixedMeanIdea::resampleButtonPressed()
{
    resetSamples();
    sample();
}

void StdevFromDistributionSamplerFixedMeanIdea::totalSamplesTextEdited(int samples)
{
    if(m_totalSamples == samples)
        return;

    resetSamples();
    setTotalSamples(samples);
}

void StdevFromDistributionSamplerFixedMeanIdea::sample()
{
    // This isn't the smartest thing in the world. -dBob.
    if(status() != IdeaStatus::Working)
        return;

    if(m_totalSamples == 0)
    {
        m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleStdevs);
        emit newData(0);
        return;
    }
    emit startSpinningGear();

    if(m_distribution.lock()->distType() == DistributionDataType::Continuous)
    {
        while(m_currentSamples < m_totalSamples)
        {
            double stdev = 0.0;
            m_samplesFromDistribution.append(QList<double>());
            m_samplesFromDistribution[m_currentSamples].reserve(m_samplesPerStdev);
            for(int i = 0; i < m_samplesPerStdev; i++)
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

                stdev += (xVal - m_mean) * (xVal - m_mean);
                m_samplesFromDistribution[m_currentSamples].append(xVal);
            }
            stdev /= m_samplesPerStdev;
            stdev = qSqrt(stdev);

            m_samplesForGridView.append(stdev);
            m_sampleStdevs.append(stdev);

            m_currentSamples++;
        }
    }
    else // Discrete
    {
        while(m_currentSamples < m_totalSamples)
        {
            double stdev = 0.0;
            m_samplesFromDistribution.append(QList<double>());
            m_samplesFromDistribution[m_currentSamples].reserve(m_samplesPerStdev);
            for(int i = 0; i < m_samplesPerStdev; i++)
            {
                double random = QRandomGenerator::global()->generateDouble();
                auto lower = std::lower_bound(m_cdf.begin(),m_cdf.end(), random);

                int index = std::distance(m_cdf.begin(), lower);

                double xVal = m_xValues[index];

                stdev += (xVal - m_mean) * (xVal - m_mean);
                m_samplesFromDistribution[m_currentSamples].append(xVal);
            }

            stdev /= m_samplesPerStdev;
            stdev = qSqrt(stdev);

            m_samplesForGridView.append(stdev);
            m_sampleStdevs.append(stdev);

            m_currentSamples++;
        }
    }

    emit startAnimation();

    emit samplesChanged(m_samplesForGridView);

    m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleStdevs);
    emit newData(0);
    emit stopSpinningGear();
}

QList<double> StdevFromDistributionSamplerFixedMeanIdea::getPositionsForDots(int i)
{
    QList<double> positions;
    positions.reserve(m_samplesPerStdev);

    if(m_samplesFromDistribution.isEmpty())
    {
        QList<double> nullList;
        nullList.reserve(m_samplesPerStdev);
        for(int j = 0; j < m_samplesPerStdev; j++)
            nullList.append(-1.0);
        return nullList;
    }

    QList<double> rawValues = m_samplesFromDistribution[i];
    double length = m_display->xHigh() - m_display->xLow();

    for(int j = 0; j < m_samplesPerStdev; j++)
    {
        double xValue = 450 * (rawValues[j] - m_display->xLow()) / length;
        positions.append(xValue);
    }

    return positions;
}

double StdevFromDistributionSamplerFixedMeanIdea::getPositionForMeanLine()
{
    return 450 * (m_mean - m_display->xLow()) / (m_display->xHigh() - m_display->xLow());
}

double StdevFromDistributionSamplerFixedMeanIdea::getStdevLineLength(int i)
{
    return 450 * (m_sampleStdevs.at(i)) / (m_display->xHigh() - m_display->xLow());
}

void StdevFromDistributionSamplerFixedMeanIdea::resetSamples()
{
    m_currentSamples = 0;
    emit resetAnimation();

    m_samplesForGridView.clear();
    emit samplesChanged(m_samplesForGridView);
    m_sampleStdevs.clear();
    m_samplesFromDistribution.clear();

    m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleStdevs);
    emit newData(0);
}

std::shared_ptr<DataType> StdevFromDistributionSamplerFixedMeanIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_samplesPointer);
}
