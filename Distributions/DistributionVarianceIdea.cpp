#include "DistributionVarianceIdea.h"
#include "QtMath"
#include "DistributionCommonFunctions.h"

DistributionVarianceAttributes::DistributionVarianceAttributes()
{
    m_listName = "Distribution Variance";
    m_displayName = "Variance";
    m_authors << "dBob";
    m_description = "Calculates the standard deviation of the input distribution";
    m_iconPath = "qrc:/Icons/DistributionVariance.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_minimizable = true;
    m_minimizedName = "Variance";
    m_differentNameOnMinimized = true;
    m_minimizedQMLPath = ":/QML/DistributionVarianceMinimized.qml";
    m_embeddedQMLPath = ":/QML/DistributionVarianceIdea.qml";

    m_externalQMLPaths << ":/QML/DistributionMeanControls.qml";
    m_externalQMLLocations.append({true, false, 10,0});
}

DistributionVarianceIdea::DistributionVarianceIdea():
    m_mean(0),
    m_variance(0),
    m_numDots(10),
    m_animationSpeed(1.0)
{

}

const IdeaAttributes DistributionVarianceIdea::attributes()
{
    return Magic<DistributionVarianceAttributes>::cast();
}

void DistributionVarianceIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

void DistributionVarianceIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    m_dist = std::dynamic_pointer_cast<DistributionDataType>(dataType);

    validate();
}

std::shared_ptr<DataType> DistributionVarianceIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_output);
}

QJsonObject DistributionVarianceIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();
    saveJson["n"] = m_numDots;
    saveJson["s"] = m_animationSpeed;

    return saveJson;
}

void DistributionVarianceIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["d"];
    if(!v.isUndefined())
    {
        m_display->load(v.toObject());
    }

    v = p["n"];
    if(!v.isUndefined())
    {
        m_numDots = v.toInt();
        emit numDotsChanged(m_numDots);
    }

    v = p["s"];
    if(!v.isUndefined())
    {
        m_animationSpeed = v.toDouble();
        emit animationSpeedChanged(m_animationSpeed);
    }

    validate();
}

void DistributionVarianceIdea::resetDisplay()
{
    if(valid())
    {
        m_display->plot();
        emit startAnimation(m_distributionType == DistributionDataType::Continuous);
    }
}

double DistributionVarianceIdea::mean() const
{
    return m_mean;
}

double DistributionVarianceIdea::variance() const
{
    return m_variance;
}

int DistributionVarianceIdea::numDots() const
{
    return m_numDots;
}

double DistributionVarianceIdea::animationSpeed() const
{
    return m_animationSpeed;
}

void DistributionVarianceIdea::setBound(double bound, int index)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setBound(bound, index);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);

    validate();
}

void DistributionVarianceIdea::setAutoFit(bool autoFit)
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

void DistributionVarianceIdea::setMean(double mean)
{
    if(qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void DistributionVarianceIdea::setVariance(double variance)
{
    if(qFuzzyCompare(m_variance, variance))
        return;

    m_variance = variance;
    emit varianceChanged(m_variance);
}

void DistributionVarianceIdea::setNumDots(int numDots)
{
    if (m_numDots == numDots)
        return;

    QJsonObject oldParams;
    oldParams["n"] = m_numDots;

    m_numDots = numDots;
    emit numDotsChanged(m_numDots);

    QJsonObject newParams;
    newParams["n"] = m_numDots;

    emit reportParamsChanged(oldParams, newParams);
    if(valid())
        compute();
}

void DistributionVarianceIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

void DistributionVarianceIdea::replayButtonClicked()
{
    if(valid())
        emit startAnimation(m_distributionType == DistributionDataType::Continuous);
}

void DistributionVarianceIdea::validate()
{
    auto v_distribution = m_dist.lock();

    if(v_distribution)
    {
        if(v_distribution->getFlags() & VARIANCEUNDEFINED)
        {
            setStatus(IdeaStatus::Error, "Variance Undefined", "The input distribution has no variance");
            m_display->clearData();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating", "Calculating the variance of the input distribution");
            m_xValues = v_distribution->xValues();
            m_yValues = v_distribution->yValues();
            m_distributionType = v_distribution->distType();

            m_cdf = DistributionCommonFunctions::makeCDF(m_distributionType, m_xValues, m_yValues);
            m_display->setXValues(v_distribution->xValues());
            m_display->setYValues(v_distribution->yValues());
            m_display->setDistType(v_distribution->distType());
            m_display->plot();

            compute();
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void DistributionVarianceIdea::compute()
{
    emit startSpinningGear();

    if(m_dist.lock()->getFlags() & VARIANCESET)
    {
        double variance = m_dist.lock()->getVariance();
        if(variance != Q_INFINITY)
        {
            setVariance(variance);
            m_output = std::make_shared<NumberDataType>(variance);
            emit newData(0);
        }
        else
        {
            setStatus(IdeaStatus::Error, "Variance Infinity", "The variance of the input distribution is infinity");
            setVariance(variance);
        }
    }
    else
    {
        double e_x = 0.0;
        double e_x2 = 0.0;

        if(m_distributionType == DistributionDataType::Continuous)
        {
            for(size_t i = 1; i < m_xValues.size(); i++)
            {
                //Assume a triangle. Integrate x over the line segment.
                double slope = (m_yValues[i] - m_yValues[i-1]) / (m_xValues[i] - m_xValues[i-1]);

                e_x += 0.333333333333 * slope * (qPow(m_xValues[i],3) - qPow(m_xValues[i-1], 3));
                e_x += 0.5 * (qPow(m_xValues[i],2) - qPow(m_xValues[i-1], 2)) * (m_yValues[i-1] - slope * m_xValues[i-1]);

                e_x2 += 0.25 * slope * (qPow(m_xValues[i],4) - qPow(m_xValues[i-1], 4));
                e_x2 += 0.333333333333 * (qPow(m_xValues[i],3) - qPow(m_xValues[i-1], 3)) * (m_yValues[i-1] - slope * m_xValues[i-1]);
            }
        }
        else
        {
            for(size_t i = 0; i < m_xValues.size(); i++)
            {
                e_x += m_xValues[i] * m_yValues[i];
                e_x2 += m_xValues[i] * m_xValues[i] * m_yValues[i];
            }
        }
        if(qFuzzyIsNull(e_x))
            setMean(0.0);
        else
            setMean(e_x);

        double variance = (e_x2 - e_x*e_x);
        setVariance(variance);

        m_output = std::make_shared<NumberDataType>(variance);
        emit newData(0);
    }
    if(m_distributionType == DistributionDataType::Continuous)
        computeContinuousVisuals();
    else
        readyDiscreteVisualsForQML();

    emit stopSpinningGear();
}

void DistributionVarianceIdea::computeContinuousVisuals()
{
    m_rectangleBounds.clear();
    m_rectangleHeights.clear();

    m_rectangleBounds.reserve(m_numDots + 1);
    m_rectangleHeights.reserve(m_numDots);

    m_rectangleBounds.append(m_xValues[0]);

    for(int i = 1; i < m_numDots; i++)
    {
        double fraction = 1.0*i/m_numDots;
        auto lower = std::lower_bound(m_cdf.begin(), m_cdf.end(), fraction);

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
                    + 2 * deltaY_deltaX * (fraction - m_cdf[index - 1]))) / (deltaY_deltaX);
        }
        else
        {
            xVal = m_xValues[index - 1] + (fraction - m_cdf[index - 1]) / m_yValues[index-1];
        }

        m_rectangleBounds.append(xVal);
    }
    m_rectangleBounds.append(m_xValues[m_xValues.size() - 1]);

    //The total area of all rectangles is 1. Since they have the same area, A = 1/m_numDots;
    //A = bh. h = A/b.
    for(int i = 0; i < m_numDots; i++)
    {
        double height = 1.0 / m_numDots / (m_rectangleBounds[i+1] - m_rectangleBounds[i]);
        m_rectangleHeights.append(height);
    }

    readyContinuousVisualsForQML();
}

void DistributionVarianceIdea::readyContinuousVisualsForQML()
{
    double lengthScale = 450 / (m_display->xHigh() - m_display->xLow());
    double heightScale = 300 / (m_display->yHigh() - m_display->yLow());

    m_rectangles.clear();
    for(int i = 0; i < m_numDots; i++)
    {
        QRectF rect((m_rectangleBounds[i] - m_display->xLow()) * lengthScale,
                    300 - (m_rectangleHeights[i] * heightScale),
                    (m_rectangleBounds[i+1] - m_rectangleBounds[i]) * lengthScale,
                m_rectangleHeights[i] * heightScale);
        m_rectangles.append(rect);
    }

    emit startAnimation(true);
}

QRectF DistributionVarianceIdea::getRectangleParams(int i)
{
    return m_rectangles.at(i);
}

void DistributionVarianceIdea::readyDiscreteVisualsForQML()
{
    setStatus(IdeaStatus::DisplayError, "TODO FIX", "TODO FIX");
}
