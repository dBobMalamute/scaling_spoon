#include "NumbersVarianceIdea.h"
#include <QDebug>
#include <QtMath>

NumbersVarianceAttributes::NumbersVarianceAttributes()
{
    m_listName = "Numbers Variance";
    m_displayName = "Variance";
    m_authors << "dBob";
    m_description = "Takes in a list of numbers and calculates the variance. Contains a display to show"
                    " what's going on inside.";
    m_iconPath = "qrc:/Icons/NumbersVariance.png";

    m_dataTypesIn << Magic<NumbersDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/NumbersVarianceIdea.qml";
    m_minimizable = true;
    m_minimizedQMLPath = ":/QML/NumbersVarianceMinimized.qml";
    m_minimizedName = "Variance";
    m_differentNameOnMinimized = true;

    m_externalQMLPaths << ":/QML/NumbersMeanControls.qml";
    m_externalQMLLocations.append({true, false, 10,0});
}

const IdeaAttributes NumbersVarianceIdea::attributes()
{
    return Magic<NumbersVarianceAttributes>::cast();
}

NumbersVarianceIdea::NumbersVarianceIdea() :
    m_lowBound(0.0),
    m_upBound(1.0),
    m_autoFit(true),
    m_mean(0),
    m_variance(0),
    m_animationSpeed(1.0)
{

}


QJsonObject NumbersVarianceIdea::save() const
{
    QJsonObject saveJson = Idea::save();

    saveJson["l"] = m_lowBound;
    saveJson["u"] = m_upBound;
    saveJson["a"] = m_autoFit;
    saveJson["s"] = m_animationSpeed;

    return saveJson;
}

void NumbersVarianceIdea::load(const QJsonObject &obj)
{
    QJsonValue v = obj["l"];
    if(!v.isUndefined())
    {
        m_lowBound = v.toDouble();
        emit lowBoundChanged(m_lowBound);
    }

    v = obj["u"];
    if(!v.isUndefined())
    {
        m_upBound = v.toDouble();
        emit upBoundChanged(m_upBound);
    }

    v = obj["a"];
    if(!v.isUndefined())
    {
        m_autoFit = v.toBool();
        emit autoFitChanged(m_autoFit);
    }

    v = obj["s"];
    if(!v.isUndefined())
    {
        m_animationSpeed = v.toDouble();
        emit animationSpeedChanged(m_animationSpeed);
    }

    validate();
}

void NumbersVarianceIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    m_inputNums = std::dynamic_pointer_cast<NumbersDataType>(dataType);

    validate();
}

std::shared_ptr<DataType> NumbersVarianceIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_outputVariance);
}

void NumbersVarianceIdea::resetDisplay()
{
    validate();
}

double NumbersVarianceIdea::lowBound() const
{
    return m_lowBound;
}

double NumbersVarianceIdea::upBound() const
{
    return m_upBound;
}

double NumbersVarianceIdea::mean() const
{
    return m_mean;
}

double NumbersVarianceIdea::variance() const
{
    return m_variance;
}

bool NumbersVarianceIdea::autoFit() const
{
    return m_autoFit;
}

double NumbersVarianceIdea::animationSpeed() const
{
    return m_animationSpeed;
}

void NumbersVarianceIdea::setLowBound(double lowBound)
{
    if (qFuzzyCompare(m_lowBound, lowBound))
        return;

    QJsonObject oldParams = Idea::save();
    oldParams["l"] = m_lowBound;
    oldParams["a"] = m_autoFit;

    m_lowBound = lowBound;
    emit lowBoundChanged(m_lowBound);

    m_autoFit = false;
    emit autoFitChanged(m_autoFit);

    QJsonObject newParams = Idea::save();
    newParams["l"] = m_lowBound;
    newParams["a"] = false;
    emit reportParamsChanged(oldParams, newParams);

    validate();
}

void NumbersVarianceIdea::setUpBound(double upBound)
{
    if (qFuzzyCompare(m_upBound, upBound))
        return;

    QJsonObject oldParams = Idea::save();
    oldParams["u"] = m_upBound;
    oldParams["a"] = m_autoFit;

    m_upBound = upBound;
    emit upBoundChanged(m_upBound);

    m_autoFit = false;
    emit autoFitChanged(m_autoFit);

    QJsonObject newParams = Idea::save();
    newParams["u"] = m_upBound;
    newParams["a"] = false;
    emit reportParamsChanged(oldParams, newParams);

    validate();
}

void NumbersVarianceIdea::setAutoFit(bool autoFit)
{
    if (m_autoFit == autoFit)
        return;

    QJsonObject oldParams = save();

    m_autoFit = autoFit;
    emit autoFitChanged(m_autoFit);
    if(m_autoFit == true)
    {
        //Make validate work
        m_lowBound = 0.0;
        m_upBound = 1.0;
        validate();
    }
    QJsonObject newParams = Idea::save();
    newParams["a"] = m_autoFit;
    emit reportParamsChanged(oldParams, newParams);
}

void NumbersVarianceIdea::replay()
{
    validate();
}

void NumbersVarianceIdea::setMean(double mean)
{
    if (qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void NumbersVarianceIdea::setVariance(double variance)
{
    if (qFuzzyCompare(m_variance, variance))
        return;

    m_variance = variance;
    emit varianceChanged(m_variance);
}

void NumbersVarianceIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

double NumbersVarianceIdea::getPositionForDot(int i)
{
    return  450 * (m_nums[i] - m_lowBound) / (m_upBound - m_lowBound);
}

void NumbersVarianceIdea::validate()
{
    auto v_nums = m_inputNums.lock();
    if(v_nums)
    {
        if(m_lowBound >= m_upBound)
        {
            setStatus(IdeaStatus::Error, "Invalid Bounds", "The lower bound is greater than or equal to the upper bound."
                                                           " Please change the bounds or set autofit to true");
            emit newDots(0);
        }
        else if(v_nums->numbers().length() == 0)
        {
            setStatus(IdeaStatus::Error, "No Input", "The input contains no numbers. It isn't possible to calculate the "
                                                     "standard deviation of nothing");
            emit newDots(0);
        }
        else if(v_nums->numbers().length() == 1)
        {
            setStatus(IdeaStatus::Error, "Only One Number", "The input has only one number. This is not allowed");
            emit newDots(0);
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating", "Currently calculating the standard"
                                                                                  "deviation of the numbers: " + v_nums->toString());
            m_nums = v_nums->numbers();
            compute();
        }
    }
    else
    {
        setDefaultStatus();
        emit newDots(0);
    }
}

void NumbersVarianceIdea::compute()
{
    emit startSpinningGear();

    double e_x = 0.0;
    double e_x2 = 0.0;

    int length = m_nums.length();
    double lowBound = m_nums[0];
    double upBound = m_nums[0];

    for (int i = 0; i < length; i++)
    {
        e_x += m_nums[i];
        e_x2 += m_nums[i] * m_nums[i];

        if(m_nums[i] < lowBound)
            lowBound = m_nums[i];
        if(m_nums[i] > upBound)
            upBound = m_nums[i];
    }
    e_x /= length;
    e_x2 /= length;

    double variance = e_x2 - e_x * e_x;
    double stdev = qSqrt(variance);

    lowBound = qMin(lowBound, e_x - stdev);
    upBound = qMax(upBound, e_x + stdev);
    //Update the properties for the display.
    if(m_autoFit)
    {
        m_lowBound = (1.01 * lowBound - 0.01 *upBound);
        emit lowBoundChanged(m_lowBound);
        m_upBound = (1.01 * upBound - 0.01 * lowBound);
        emit upBoundChanged(m_upBound);
    }
    else
    {
        if(lowBound < m_lowBound || upBound > m_upBound)
            setStatus(IdeaStatus::Warning, "Bounds too Small", "There are points which are not being shown. Consider increasing"
                                                               " the bounds to show all the points");
    }
    setMean(e_x);
    setVariance(variance);

    //Let the display know there are new dots, as well as how many there are.
    emit newDots(length);

    m_outputVariance = std::make_shared<NumberDataType>(variance);
    emit newData(0);
    emit stopSpinningGear();
}
