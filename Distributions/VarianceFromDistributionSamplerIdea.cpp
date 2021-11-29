#include "VarianceFromDistributionSamplerIdea.h"
#include <QJsonArray>
#include <QRandomGenerator>
#include <QtMath>

#include "DistributionCommonFunctions.h"

VarianceFromDistributionSamplerAttributes::VarianceFromDistributionSamplerAttributes()
{
    m_listName = "Variance from Distribution Sampler";
    m_displayName = "Variance From Distribution Sampler";
    m_authors << "dBob";
    m_description = "This box takes in a distribution and samples standard deviations from it. It features a nice visual when maximized";
    m_iconPath =  "qrc:/Icons/VarianceFromDistributionSampler.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumbersDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/VarianceFromDistributionSamplerIdea.qml";

    m_minimizable = true;
    m_startMinimized = true;
    m_minimizedQMLPath = ":/QML/VarianceFromDistributionSamplerMinimized.qml";
    m_differentNameOnMinimized = true;
    m_minimizedName = "Variance From\nDistribution Sampler";

    m_externalQMLPaths << ":/QML/StdevFromDistributionControls.qml";
    m_externalQMLLocations.append({true, false, 10,0});
}

const IdeaAttributes VarianceFromDistributionSamplerIdea::attributes()
{
    return Magic<VarianceFromDistributionSamplerAttributes>::cast();
}

void VarianceFromDistributionSamplerIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

VarianceFromDistributionSamplerIdea::VarianceFromDistributionSamplerIdea() :
    m_animationSpeed(2.72)
{
    m_samplingSize = 1;
    m_samplesForGridView.clear();
    m_samplesFromDistribution.clear();
    m_sampleVariances.clear();
    m_sampleMeans.clear();
    m_totalSamples = 0;
    m_currentSamples = 0;
    m_samplesPerStdev = 10;
}

void VarianceFromDistributionSamplerIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto distribution = std::dynamic_pointer_cast<DistributionDataType>(dataType);
    m_distribution = distribution;

    validate();
}

QJsonObject VarianceFromDistributionSamplerIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    saveJson["s"] = m_samplingSize;
    saveJson["n"] = m_totalSamples;
    saveJson["sm"] = m_samplesPerStdev;
    saveJson["as"] = m_animationSpeed;

    return saveJson;
}

void VarianceFromDistributionSamplerIdea::load(const QJsonObject &p)
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

void VarianceFromDistributionSamplerIdea::resetDisplay()
{
    if(valid())
        m_display->plot();

    emit resetAnimation();
    emit startAnimation();
    emit samplesChanged(m_samplesForGridView);
}

QList<QVariant> VarianceFromDistributionSamplerIdea::samples() const
{
    return m_samplesForGridView;
}

int VarianceFromDistributionSamplerIdea::samplingSize() const
{
    return m_samplingSize;
}

int VarianceFromDistributionSamplerIdea::samplesPerStdev() const
{
    return m_samplesPerStdev;
}

int VarianceFromDistributionSamplerIdea::totalSamples() const
{
    return m_totalSamples;
}

double VarianceFromDistributionSamplerIdea::animationSpeed() const
{
    return m_animationSpeed;
}

void VarianceFromDistributionSamplerIdea::validate()
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

void VarianceFromDistributionSamplerIdea::setBound(double bound, int index)
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

void VarianceFromDistributionSamplerIdea::setAutoFit(bool autoFit)
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
void VarianceFromDistributionSamplerIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

void VarianceFromDistributionSamplerIdea::setTotalSamples(int totalSamples)
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

void VarianceFromDistributionSamplerIdea::setSamples(QList<QVariant> samples)
{
    if (m_samplesForGridView == samples)
        return;

    m_samplesForGridView = samples;
    emit samplesChanged(m_samplesForGridView);
}

void VarianceFromDistributionSamplerIdea::setSamplingSize(int samplingSize)
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

void VarianceFromDistributionSamplerIdea::setSamplesPerStdev(int samplesPerStdev)
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

void VarianceFromDistributionSamplerIdea::sampleButtonPressed()
{
    setTotalSamples(m_totalSamples + m_samplingSize);
}

void VarianceFromDistributionSamplerIdea::resampleButtonPressed()
{
    resetSamples();
    sample();
}

void VarianceFromDistributionSamplerIdea::totalSamplesTextEdited(int samples)
{
    if(m_totalSamples == samples)
        return;

    resetSamples();
    setTotalSamples(samples);
}

void VarianceFromDistributionSamplerIdea::sample()
{
    // This isn't the smartest thing in the world. -dBob.
    if(status() != IdeaStatus::Working)
        return;

    if(m_totalSamples == 0)
    {
        m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleVariances);
        emit newData(0);
        return;
    }
    emit startSpinningGear();

    if(m_distribution.lock()->distType() == DistributionDataType::Continuous)
    {
        while(m_currentSamples < m_totalSamples)
        {
            double e_x = 0.0;
            double e_x2 = 0.0;
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

                e_x += xVal;
                e_x2 += xVal*xVal;
                m_samplesFromDistribution[m_currentSamples].append(xVal);
            }
            e_x /= m_samplesPerStdev;
            e_x2 /= m_samplesPerStdev;
            double variance = e_x2 - e_x * e_x;
            m_sampleMeans.append(e_x);

            m_samplesForGridView.append(variance);
            m_sampleVariances.append(variance);

            m_currentSamples++;
        }
    }
    else // Discrete
    {
        while(m_currentSamples < m_totalSamples)
        {
            double e_x = 0.0;
            double e_x2 = 0.0;
            m_samplesFromDistribution.append(QList<double>());
            m_samplesFromDistribution[m_currentSamples].reserve(m_samplesPerStdev);
            for(int i = 0; i < m_samplesPerStdev; i++)
            {
                double random = QRandomGenerator::global()->generateDouble();
                auto lower = std::lower_bound(m_cdf.begin(),m_cdf.end(), random);

                int index = std::distance(m_cdf.begin(), lower);

                double xVal = m_xValues[index];

                e_x += xVal;
                e_x2 += xVal * xVal;
                m_samplesFromDistribution[m_currentSamples].append(xVal);
            }
            e_x /= m_samplesPerStdev;
            e_x2 /= m_samplesPerStdev;

            double variance = e_x2 - e_x * e_x;
            m_sampleMeans.append(e_x);

            m_samplesForGridView.append(variance);
            m_sampleVariances.append(variance);

            m_currentSamples++;
        }
    }

    emit startAnimation();

    emit samplesChanged(m_samplesForGridView);

    m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleVariances);
    emit newData(0);
    emit stopSpinningGear();
}

QList<double> VarianceFromDistributionSamplerIdea::getPositionsForDots(int i)
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

double VarianceFromDistributionSamplerIdea::getPositionForMeanLine(int i)
{
    return 450 * (m_sampleMeans.at(i) - m_display->xLow()) / (m_display->xHigh() - m_display->xLow());
}

double VarianceFromDistributionSamplerIdea::getStdevLineLength(int i)
{
    return 450 * (qSqrt(m_sampleVariances.at(i))) / (m_display->xHigh() - m_display->xLow());
}

void VarianceFromDistributionSamplerIdea::resetSamples()
{
    m_currentSamples = 0;

    m_samplesForGridView.clear();
    emit samplesChanged(m_samplesForGridView);
    m_sampleVariances.clear();
    m_sampleMeans.clear();
    m_samplesFromDistribution.clear();

    emit resetAnimation();

    m_samplesPointer = std::make_shared<NumbersDataType>(m_sampleVariances);
    emit newData(0);
}

std::shared_ptr<DataType> VarianceFromDistributionSamplerIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_samplesPointer);
}
