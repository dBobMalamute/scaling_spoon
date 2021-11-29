#include "DistributionSamplerIdea.h"
#include <QJsonArray>
#include <QRandomGenerator>
#include <QtMath>

#include "DistributionCommonFunctions.h"

DistributionSamplerAttributes::DistributionSamplerAttributes()
{
    m_listName = "Distribution Sampler";
    m_displayName = "Distribution Sampler";
    m_authors << "dBob";
    m_description = "This box takes in a distribution and samples number from it. It features a nice visual when maximized";
    m_iconPath = "qrc:/Icons/DistributionSampler.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumbersDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/DistributionSamplerIdea.qml";
    m_minimizable = true;
    m_startMinimized = true;
    m_minimizedQMLPath = ":/QML/DistributionSamplerMinimized.qml";
}

const IdeaAttributes DistributionSamplerIdea::attributes()
{
    return Magic<DistributionSamplerAttributes>::cast();
}

void DistributionSamplerIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

DistributionSamplerIdea::DistributionSamplerIdea()
{
    m_totalSamples = 0;
    m_samplingSize = 1;
    emit totalSamplesChanged(m_totalSamples);
    m_samplesInImplicitBin.reserve(80);
    for(int i = 0; i < 80; i++)
        m_samplesInImplicitBin.append(0.0);
    resetSamples();
}

void DistributionSamplerIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto distribution = std::dynamic_pointer_cast<DistributionDataType>(dataType);
    m_distribution = distribution;

    validate();
}

QJsonObject DistributionSamplerIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();
    saveJson["s"] = m_samplingSize;
    saveJson["n"] = m_totalSamples;

    return saveJson;
}

void DistributionSamplerIdea::load(const QJsonObject &p)
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

}

void DistributionSamplerIdea::resetDisplay()
{
    for(int i = 0; i < 80; i++)
        m_samplesInImplicitBin[i] = 0;

    if(valid())
        m_display->plot();

    emit samplesChanged(m_samples);
    emit startAnimation();
}

QList<QVariant> DistributionSamplerIdea::samples() const
{
    return m_samples;
}

int DistributionSamplerIdea::samplingSize() const
{
    return m_samplingSize;
}

int DistributionSamplerIdea::totalSamples() const
{
    return m_totalSamples;
}

void DistributionSamplerIdea::validate()
{
    auto v_distribution = m_distribution.lock();
    if(v_distribution)
    {
        setStatus(IdeaStatus::Working, "Plotting", "Ready to sample a distribution.");
        m_xValues = v_distribution->xValues();
        m_yValues = v_distribution->yValues();
        m_distributionType = v_distribution->distType();

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


void DistributionSamplerIdea::setBound(double bound, int index)
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

void DistributionSamplerIdea::setAutoFit(bool autoFit)
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

void DistributionSamplerIdea::setTotalSamples(int totalSamples)
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

void DistributionSamplerIdea::setSamples(QList<QVariant> samples)
{
    if (m_samples == samples)
        return;

    m_samples = samples;
    emit samplesChanged(m_samples);
}

void DistributionSamplerIdea::setSamplingSize(int samplingSize)
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

void DistributionSamplerIdea::sampleButtonPressed()
{
    setTotalSamples(m_totalSamples + m_samplingSize);
}

void DistributionSamplerIdea::resampleButtonPressed()
{
    resetSamples();
    sample();
}

void DistributionSamplerIdea::totalSamplesTextEdited(int samples)
{
    if(m_totalSamples == samples)
        return;

    resetSamples();
    setTotalSamples(samples);
}

void DistributionSamplerIdea::sample()
{
    // I think I should probably make status a property so I can use it from QML.
    // This isn't the smartest thing in the world. -dBob.
    if(status() != IdeaStatus::Working)
        return;

    if(m_totalSamples == 0)
    {
        m_samplesPointer = std::make_shared<NumbersDataType>(m_samplesDoubles);
        emit newData(0);
        return;
    }
    emit startSpinningGear();

    if(m_distribution.lock()->distType() == DistributionDataType::Continuous)
    {
        while(m_currentSamples < m_totalSamples)
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

            m_samples.append(xVal);
            m_samplesDoubles.append(xVal);
            m_currentSamples++;
        }
    }
    else // Discrete
    {
        while(m_currentSamples < m_totalSamples)
        {
            double random = QRandomGenerator::global()->generateDouble();
            auto lower = std::lower_bound(m_cdf.begin(),m_cdf.end(), random);

            int index = std::distance(m_cdf.begin(), lower);

            double xVal = m_xValues[index];

            m_samples.append(xVal);
            m_samplesDoubles.append(xVal);
            m_currentSamples++;
        }
    }

    emit startAnimation();

    emit samplesChanged(m_samples);

    m_samplesPointer = std::make_shared<NumbersDataType>(m_samplesDoubles);
    emit newData(0);
    emit stopSpinningGear();
}

QPointF DistributionSamplerIdea::getPositionForDot(int i)
{
    //450 is the width. Position is percentage.
    double xValue = 450 * (m_samplesDoubles[i] - m_display->xLow())/
            (m_display->xHigh() - m_display->xLow());
    int implicitBinIndex = qFloor((80 * xValue / 450.00001));
    double yValue = 290 - 2.0 * m_samplesInImplicitBin.at(implicitBinIndex);
    m_samplesInImplicitBin[implicitBinIndex]++;

    return QPointF(xValue, yValue);
}

void DistributionSamplerIdea::resetSamples()
{
    m_currentSamples = 0;

    for(int i = 0; i < 80; i++)
        m_samplesInImplicitBin[i] = 0;

    m_samples.clear();
    emit samplesChanged(m_samples);
    m_samplesDoubles.clear();

    emit resetAnimation();

    m_samplesPointer = std::make_shared<NumbersDataType>(m_samplesDoubles);
    emit newData(0);
}

std::shared_ptr<DataType> DistributionSamplerIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_samplesPointer);
}
