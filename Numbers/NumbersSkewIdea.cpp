#include "NumbersSkewIdea.h"
#include <QDebug>
#include <QtMath>

NumbersSkewIdeaAttributes::NumbersSkewIdeaAttributes()
{
    m_listName = "Numbers Skew";
    m_displayName = "Skew";
    m_authors << "dBob";
    m_description = "Takes in a list of numbers and calculates the skew. Contains a display to show"
                    " what's going on inside.";
    m_iconPath = "";//"qrc:/Icons/NumbersMean.png";

    m_dataTypesIn << Magic<NumbersDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_gear = true;

    m_embeddedQMLPath = ":/QML/NumbersSkewMinimized.qml";
//    m_embeddedQMLPath = ":/QML/NumbersMeanIdea.qml";
//    m_minimizable = true;
//    m_minimizedQMLPath = ":/QML/NumbersMeanMinimized.qml";

//    m_externalQMLPaths << ":/QML/NumbersMeanControls.qml";
//    m_externalQMLLocations.append({true, false, 10,0});
}

const IdeaAttributes NumbersSkewIdea::attributes()
{
    return Magic<NumbersSkewIdeaAttributes>::cast();
}

NumbersSkewIdea::NumbersSkewIdea() :
    m_lowBound(0.0),
    m_upBound(1.0),
    m_autoFit(true),
    m_mean(0),
    m_skew(0),
    m_animationSpeed(1.0)
{

}


QJsonObject NumbersSkewIdea::save() const
{
    QJsonObject saveJson = Idea::save();

    saveJson["l"] = m_lowBound;
    saveJson["u"] = m_upBound;
    saveJson["a"] = m_autoFit;
    saveJson["s"] = m_animationSpeed;

    return saveJson;
}

void NumbersSkewIdea::load(const QJsonObject &obj)
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

void NumbersSkewIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    m_inputNums = std::dynamic_pointer_cast<NumbersDataType>(dataType);

    validate();
}

std::shared_ptr<DataType> NumbersSkewIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_outputSkew);
}

void NumbersSkewIdea::resetDisplay()
{
    if(valid())
    {
        emit newDots(m_nums.length());
    }
}

double NumbersSkewIdea::lowBound() const
{
    return m_lowBound;
}

double NumbersSkewIdea::upBound() const
{
    return m_upBound;
}

double NumbersSkewIdea::mean() const
{
    return m_mean;
}

bool NumbersSkewIdea::autoFit() const
{
    return m_autoFit;
}

double NumbersSkewIdea::animationSpeed() const
{
    return m_animationSpeed;
}

double NumbersSkewIdea::skew() const
{
    return m_skew;
}

void NumbersSkewIdea::setLowBound(double lowBound)
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

void NumbersSkewIdea::setUpBound(double upBound)
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

void NumbersSkewIdea::setAutoFit(bool autoFit)
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

void NumbersSkewIdea::replay()
{
    resetDisplay();
}

void NumbersSkewIdea::setMean(double mean)
{
    if (qFuzzyCompare(m_mean, mean))
        return;

    m_mean = mean;
    emit meanChanged(m_mean);
}

void NumbersSkewIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

void NumbersSkewIdea::setSkew(double skew)
{
    if (qFuzzyCompare(m_skew, skew))
        return;

    m_skew = skew;
    emit skewChanged(m_skew);
}

double NumbersSkewIdea::getPositionForDot(int i)
{
    return  450 * (m_nums[i] - m_lowBound) / (m_upBound - m_lowBound);
}

void NumbersSkewIdea::validate()
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
                                                     "skew of nothing");
            emit newDots(0);
        }
        else if(v_nums->numbers().length() <= 2)
        {
            setStatus(IdeaStatus::Error, "Not enough Numbers", "The input has less than 3 numbers. Skew"
                                                                 " is not defined.");
            m_nums = v_nums->numbers();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Calculating the Skew", "Calculating the skew");
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

void NumbersSkewIdea::compute()
{
    emit startSpinningGear();

    double mean = m_nums[0];
    int length = m_nums.length();

    double lowBound = m_nums[0];
    double upBound = m_nums[0];
    for (int i = 1; i < length; i++)
    {
        mean += m_nums[i];

        if(m_nums[i] < lowBound)
            lowBound = m_nums[i];
        if(m_nums[i] > upBound)
            upBound = m_nums[i];
    }
    mean /= length;

    double stdev = 0.0;
    double skew = 0.0;

    for(int i = 0; i < length; i++)
    {
        double temp = m_nums[i] - mean;
        stdev += temp*temp;
        skew += temp*temp*temp;
    }
    if(stdev == 0)
    {
        setStatus(IdeaStatus::Error, "Stdev = 0", "The skew is undefined if the standard deviation is 0");
        emit stopSpinningGear();
        return;
    }
    stdev = qSqrt(stdev / (length - 1));
    skew /= (length * qPow(stdev,3));

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
    setMean(mean);
    setSkew(skew);

    //Let the display know there are new dots, as well as how many there are.
    emit newDots(length);

    m_outputSkew = std::make_shared<NumberDataType>(skew);
    emit newData(0);
    emit stopSpinningGear();
}

