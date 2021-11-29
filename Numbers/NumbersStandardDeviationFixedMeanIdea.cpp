#include "NumbersStandardDeviationFixedMeanIdea.h"
#include <QDebug>
#include <QtMath>

NumbersStandardDeviationFixedMeanAttributes::NumbersStandardDeviationFixedMeanAttributes()
{
    m_listName = "Numbers Standard Deviation with Fixed Mean";
    m_displayName = "Standard Deviation with Fixed Mean";
    m_authors << "dBob";
    m_description = "Takes in a list of numbers and calculates the standard deviation. Contains a display to show"
                    " what's going on inside.";
    m_iconPath = "qrc:/Icons/NumbersStandardDeviationFixedMean.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumbersDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_labelsIn << "Mean" << "Nums";
    m_labelsOut << "#";
    m_customLabels = true;

    m_gear = true;

    m_embeddedQMLPath = ":/QML/NumbersStandardDeviationIdea.qml";
    m_minimizable = true;
    m_minimizedQMLPath = ":/QML/NumbersStandardDeviationMinimized.qml";
    m_minimizedName = "Stdev\nFixed Mean";
    m_differentNameOnMinimized = true;

    m_externalQMLPaths << ":/QML/NumbersMeanControls.qml";
    m_externalQMLLocations.append({true, false, 10,0});
}

const IdeaAttributes NumbersStandardDeviationFixedMeanIdea::attributes()
{
    return Magic<NumbersStandardDeviationFixedMeanAttributes>::cast();
}

NumbersStandardDeviationFixedMeanIdea::NumbersStandardDeviationFixedMeanIdea() :
    m_lowBound(0.0),
    m_upBound(1.0),
    m_autoFit(true),
    m_mean(0),
    m_stdev(0),
    m_animationSpeed(1.0)
{

}


QJsonObject NumbersStandardDeviationFixedMeanIdea::save() const
{
    QJsonObject saveJson = Idea::save();

    saveJson["l"] = m_lowBound;
    saveJson["u"] = m_upBound;
    saveJson["a"] = m_autoFit;
    saveJson["s"] = m_animationSpeed;

    return saveJson;
}

void NumbersStandardDeviationFixedMeanIdea::load(const QJsonObject &obj)
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

void NumbersStandardDeviationFixedMeanIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    if(plugNumber == 1)
        m_inputNums = std::dynamic_pointer_cast<NumbersDataType>(dataType);
    else
        m_inputMean = std::dynamic_pointer_cast<NumberDataType>(dataType);

    validate();
}

std::shared_ptr<DataType> NumbersStandardDeviationFixedMeanIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_outputStdev);
}

void NumbersStandardDeviationFixedMeanIdea::resetDisplay()
{
    validate();
}

double NumbersStandardDeviationFixedMeanIdea::lowBound() const
{
    return m_lowBound;
}

double NumbersStandardDeviationFixedMeanIdea::upBound() const
{
    return m_upBound;
}

double NumbersStandardDeviationFixedMeanIdea::mean() const
{
    return m_mean;
}

double NumbersStandardDeviationFixedMeanIdea::stdev() const
{
    return m_stdev;
}

bool NumbersStandardDeviationFixedMeanIdea::autoFit() const
{
    return m_autoFit;
}

double NumbersStandardDeviationFixedMeanIdea::animationSpeed() const
{
    return m_animationSpeed;
}

void NumbersStandardDeviationFixedMeanIdea::setLowBound(double lowBound)
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

void NumbersStandardDeviationFixedMeanIdea::setUpBound(double upBound)
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

void NumbersStandardDeviationFixedMeanIdea::setAutoFit(bool autoFit)
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

void NumbersStandardDeviationFixedMeanIdea::replay()
{
    validate();
}

void NumbersStandardDeviationFixedMeanIdea::setMean(double mean)
{
    if (qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void NumbersStandardDeviationFixedMeanIdea::setStdev(double stdev)
{
    if (qFuzzyCompare(m_stdev, stdev))
        return;

    m_stdev = stdev;
    emit stdevChanged(m_stdev);
}

void NumbersStandardDeviationFixedMeanIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

double NumbersStandardDeviationFixedMeanIdea::getPositionForDot(int i)
{
    return  450 * (m_nums[i] - m_lowBound) / (m_upBound - m_lowBound);
}

void NumbersStandardDeviationFixedMeanIdea::validate()
{
    auto v_nums = m_inputNums.lock();
    auto v_mean = m_inputMean.lock();
    if(v_nums && v_mean)
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
            m_mean = v_mean->number();
            compute();
        }
    }
    else
    {
        setDefaultStatus();
        emit newDots(0);
    }
}

void NumbersStandardDeviationFixedMeanIdea::compute()
{
    emit startSpinningGear();

    double stdev = 0.0;

    int length = m_nums.length();
    double lowBound = m_nums[0];
    double upBound = m_nums[0];

    for (int i = 0; i < length; i++)
    {
        stdev += (m_nums[i] - m_mean) * (m_nums[i] - m_mean);

        if(m_nums[i] < lowBound)
            lowBound = m_nums[i];
        if(m_nums[i] > upBound)
            upBound = m_nums[i];
    }
    stdev /= length;

    stdev = qSqrt(stdev);

    lowBound = qMin(lowBound, m_mean - stdev);
    upBound = qMax(upBound, m_mean + stdev);
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
    setStdev(stdev);

    //Let the display know there are new dots, as well as how many there are.
    emit newDots(length);

    m_outputStdev = std::make_shared<NumberDataType>(stdev);
    emit newData(0);
    emit stopSpinningGear();
}

