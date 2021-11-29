#include "NumbersBinnerIdea.h"
#include <QDebug>

#include <QJsonArray>

NumbersBinnerAttributes::NumbersBinnerAttributes()
{
    m_listName = "Numbers Binner";
    m_displayName = "Numbers Binner";
    m_authors << "dBob";
    m_description = "This box turns a list of numbers into a bar-style chart";
    m_iconPath = "qrc:/Icons/NumbersBinner.png";

    m_dataTypesIn << Magic<NumbersDataTypeAttributes>::cast().id();
    m_labelsIn << "Nums";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/NumbersBinnerIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Binner";
    m_differentNameOnMinimized = true;

    m_externalQMLPaths << ":/QML/NumbersBinnerControls.qml"
                       << ":/QML/NumbersBinnerControls2.qml";
    m_externalQMLLocations.append({true, false, 10,0});
    m_externalQMLLocations.append({true, false, 10,120});

    m_gear = true;
}

const IdeaAttributes NumbersBinnerIdea::attributes()
{
    return Magic<NumbersBinnerAttributes>::cast();
}

NumbersBinnerIdea::NumbersBinnerIdea() :
    m_autoFitGrid(true),
    m_binBounds(),
    m_numBins(6),
    m_lowBinBoundary(0),
    m_upBinBoundary(4),
    m_animationSpeed(2.72)
{
    m_gridBounds << -5.0 << 5.0 << 0.0 << 1.0;
    m_binBounds.reserve(4);
    m_binBounds << 0.00 << 1.0 << 2.0 << 3.0 << 4.0;
}

QJsonObject NumbersBinnerIdea::save() const
{
    QJsonObject saveJson = Idea::save();

    QJsonArray gridboundsArray;
    for(int i = 0; i < 4; i++)
        gridboundsArray.append(m_gridBounds.at(i));
    saveJson["gb"] = gridboundsArray;

    saveJson["a"] = m_autoFitGrid;

    QJsonArray binBoundsArray;
    for(int i = 0; i < m_binBounds.length(); i++)
    {
        binBoundsArray.append(m_binBounds.at(i));
    }
    saveJson["bb"] = binBoundsArray;

    saveJson["n"] = m_numBins;
    saveJson["l"] = m_lowBinBoundary;
    saveJson["u"] = m_upBinBoundary;
    saveJson["s"] = m_animationSpeed;

    return saveJson;
}

void NumbersBinnerIdea::load(const QJsonObject &obj)
{
    QJsonValue v;

    v = obj["gb"];
    if(!v.isUndefined())
    {
        QJsonArray gridBoundsArray = v.toArray();
        for(int i = 0; i < 4; i++)
        {
            m_gridBounds[i] = gridBoundsArray.at(i).toDouble();
        }
        emit gridBoundsChanged(m_gridBounds);
    }

    v = obj["a"];
    if(!v.isUndefined())
    {
        m_autoFitGrid = v.toBool();
        emit autoFitGridChanged(m_autoFitGrid);
    }

    v = obj["bb"];
    if(!v.isUndefined())
    {
        QJsonArray boundsArray = v.toArray();
        m_binBounds.clear();
        m_binBounds.reserve(boundsArray.size());
        for(int i = 0; i < boundsArray.size(); i++)
        {
            m_binBounds.append(boundsArray.at(i).toDouble());
        }
        emit binBoundsChanged(m_binBounds);
    }

    v = obj["n"];
    if(!v.isUndefined())
    {
        m_numBins = v.toInt();
        emit numBinsChanged(m_numBins);
    }

    v = obj["l"];
    if(!v.isUndefined())
    {
        m_lowBinBoundary = v.toDouble();
        emit lowBinBoundaryChanged(m_lowBinBoundary);
    }

    v = obj["u"];
    if(!v.isUndefined())
    {
        m_upBinBoundary = v.toDouble();
        emit upBinBoundaryChanged(m_upBinBoundary);
    }

    v = obj["s"];
    if(!v.isUndefined())
    {
        m_animationSpeed = v.toDouble();
        emit animationSpeedChanged(m_animationSpeed);
    }

    validate();
}

void NumbersBinnerIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    m_nums = std::dynamic_pointer_cast<NumbersDataType>(dataType);

    validate();
}

void NumbersBinnerIdea::resetDisplay()
{
    emit binBoundsChanged(m_binBounds);
    if(status() == IdeaStatus::Working || status() == IdeaStatus::DisplayError)
    {
        emit newDots(m_dotInitialPositions.size());
    }
}

void NumbersBinnerIdea::validate()
{
    auto v_nums = m_nums.lock();
    if(v_nums)
    {
        if(m_binBounds.length() == 0)
        {
            setStatus(IdeaStatus::StandBy, "Enter Bounds", "Please enter the bounds you wish to turn the input set of "
                                                           "numbers into.");
            emit newDots(0);
        }
        else
        {
            // Are the bounds in the correct order.
            bool valid = true;
            for(int i = 1; i < m_binBounds.size(); i++)
            {
                if(m_binBounds[i] <= m_binBounds[i-1])
                    valid = false;
            }

            if(!valid)
            {
                setStatus(IdeaStatus::Error, "Invalid Bounds", "The bounds must be ordered from lowest to highest");
                emit newDots(0);
            }
            else
            {
                if(m_nums.lock().get()->numbers().length() == 0)
                {
                    setStatus(IdeaStatus::StandBy, "No Input", "The box is not currently being given any input.");
                    emit newDots(0);
                }
                else
                {
                    setStatus(IdeaStatus::Working, "Binning Numbers", "Currently binning the input numbers");
                    compute();
                }
            }
        }
    }
    else
    {
        setDefaultStatus();
        emit newDots(0);
    }
}

void NumbersBinnerIdea::compute()
{
    emit startSpinningGear();

    QList<double> input = m_nums.lock().get()->numbers();
    double inputSize = input.size();

    //Fit or Check the bounds.
    double lowBound = input[0];
    double upBound = input[0];
    for(int i = 1 ; i < inputSize; i++)
    {
        if(input[i] > upBound)
            upBound = input[i];
        if(input[i] < lowBound)
            lowBound = input[i];
    }

    //Compare to the bin boundaries.
    if(m_binBounds.length() == 1)
    {
        if(lowBound > m_binBounds[0])
            lowBound = m_binBounds[0];
        if(upBound < m_binBounds[0])
            upBound = m_binBounds[0];

        if(m_autoFitGrid)
        {
            double width = upBound - lowBound;
            m_gridBounds[0] = lowBound - 0.025 * width;
            m_gridBounds[1] = upBound + 0.025 * width;
            emit gridBoundsChanged(m_gridBounds);
        }
        else
        {
            if(lowBound < m_gridBounds[0] || upBound > m_gridBounds[1])
                setStatus(IdeaStatus::DisplayError,"Display Error?", "The grid bounds might not contain all the data. "
                                                                     "This can be misleading, consider turning autofit on.");
        }

    }
    else
    {
        if(m_autoFitGrid)
        {
            double width = (m_binBounds[m_binBounds.length()-1] - m_binBounds[0]) / (m_binBounds.length() - 1);
            if(lowBound > m_binBounds[0] - width)
                lowBound = m_binBounds[0] - width;
            if(upBound < m_binBounds[m_binBounds.length()-1] + width)
                upBound = m_binBounds[m_binBounds.length()-1] + width;
            m_gridBounds[0] = lowBound;
            m_gridBounds[1] = upBound;
            emit gridBoundsChanged(m_gridBounds);
        }
        else
        {
            if(lowBound < m_gridBounds[0] || upBound > m_gridBounds[1])
                setStatus(IdeaStatus::DisplayError,"Display Error?", "The grid bounds might not contain all the data. "
                                                                     "This can be misleading, consider turning autofit on.");
        }
    }

    //Prepare binHeights.
    QList<double> binHeights;
    double size = m_binBounds.size();
    binHeights.reserve(size + 1);
    for(int i = 0; i < size + 1; i++)
    {
        binHeights.append(0.0);
    }

    //Prepare binWidths, place to put the first dot, etc
    m_binWidths.clear();

    m_binWidths.reserve(size + 1);

    int width = 450 * (m_binBounds[0] - m_gridBounds[0]) / (m_gridBounds[1] - m_gridBounds[0]);
    if(width != 0)
        m_binWidths.append(width);
    else
        m_binWidths.append(1);
    for(int i = 1; i < size; i++)
    {
        m_binWidths.append((int) 450 *(m_binBounds[i] - m_binBounds[i-1]) / (m_gridBounds[1] - m_gridBounds[0]));
    }
    width = 450 * (m_gridBounds[1] - m_binBounds[size - 1]) / (m_gridBounds[1] - m_gridBounds[0]);
    if(width != 0)
        m_binWidths.append(width);
    else
        m_binWidths.append(1);

    //Add maximum to avoid having to check if j is valid in loop.
    //If i didn't do this, I'd need an extra if statement in the while loop.
    m_binBounds.append(1.7976931348623157E+308);

    //Calculate everything for the moving dots.
    m_dotInitialPositions.clear();
    m_dotFinalPositions.clear();
    m_dotInitialPositions.reserve(inputSize);
    m_dotFinalPositions.reserve(inputSize);

    for(int i = 0; i < inputSize; i++)
    {
        //Set the initial position for the first dot.
        double initPos = 0 + 450 * (input[i] - m_gridBounds[0]) / (m_gridBounds[1] - m_gridBounds[0]);
        m_dotInitialPositions.append(initPos);

        int j = 0;
        while(input[i] > m_binBounds[j])
            j++;
        binHeights[j]++;

        int gridSquares = ((m_binWidths[j] - 2) / 5);

        int xShift = 6 + 5 * (((int)binHeights[j] - 1) % gridSquares);
        int yShift = 5 * (((int)binHeights[j] - 1) / gridSquares);
        m_dotFinalPositions.append({getPositionForBoundLine(j -1) + xShift, static_cast<double>(qMax(296 - yShift, 6))});
    }

    //Remove maximum to avoid having to check if j is valid in loop.
    m_binBounds.removeLast();

    double highBound = -1.0;
    //Normalize the bin heights
    for(int i = 0; i < size + 1; i++)
    {
        binHeights[i] /= inputSize;

        if(binHeights[i] > highBound)
            highBound = binHeights[i];
    }

    //Part of making it look pretty
    if(m_autoFitGrid)
    {
        double height = highBound - 0;
        m_gridBounds[2] = 0;
        m_gridBounds[3] = highBound + 0.025 * height;
        emit gridBoundsChanged(m_gridBounds);
    }
    else
    {
        if( m_gridBounds[2] > 0 || m_gridBounds[3] < highBound)
            setStatus(IdeaStatus::DisplayError,"Display Error?", "The grid bounds might not contain all the data. "
                                                                 "This can be misleading, consider turning autofit on.");
    }


    m_binHeights = binHeights;
    emit newDots(inputSize);

    emit stopSpinningGear();
}

QList<double> NumbersBinnerIdea::binBounds() const
{
    return m_binBounds;
}

int NumbersBinnerIdea::numBins() const
{
    return m_numBins;
}

double NumbersBinnerIdea::lowBinBoundary() const
{
    return m_lowBinBoundary;
}

double NumbersBinnerIdea::upBinBoundary() const
{
    return m_upBinBoundary;
}

double NumbersBinnerIdea::animationSpeed() const
{
    return m_animationSpeed;
}

bool NumbersBinnerIdea::autoFitGrid() const
{
    return m_autoFitGrid;
}

QList<double> NumbersBinnerIdea::gridBounds() const
{
    return m_gridBounds;
}

void NumbersBinnerIdea::setBinBounds(QList<double> bounds)
{
    if (m_binBounds == bounds)
        return;

    QJsonObject oldParams = Idea::save();
    QJsonArray oldBoundsArray;
    for(int i = 0; i < m_binBounds.size(); i++)
        oldBoundsArray.append(m_binBounds.at(i));
    oldParams["bb"] = oldBoundsArray;

    m_binBounds = bounds;
    emit binBoundsChanged(m_binBounds);

    QJsonObject newParams = Idea::save();
    QJsonArray newBoundsArray;
    for(int i = 0; i < m_binBounds.size(); i++)
        newBoundsArray.append(m_binBounds.at(i));
    newParams["bb"] = newBoundsArray;

    emit reportParamsChanged(oldParams, newParams);
}

void NumbersBinnerIdea::sanitizeInputBounds(QString input)
{
    //Split the list into sublists:
    QStringList inputList = input.split(QLatin1Char(','), Qt::SkipEmptyParts);

    //Remove whitespace
    foreach(QString str, inputList)
    {
        str = str.trimmed();
    }

    bool ok = true;
    QList<double> newBounds;
    newBounds.reserve(inputList.size());
    foreach(QString str, inputList)
    {
        double temp = str.toDouble(&ok);
        if(ok)
            newBounds.append(temp);
        else
            break;
    }

    if(ok)
    {
        setBinBounds(newBounds);
        validate();
    }
}

void NumbersBinnerIdea::setBoundsByButton()
{
    QList<double> newBounds;
    if(m_numBins == 2)
    {
        newBounds.append(m_upBinBoundary + m_lowBinBoundary / 2.0);
        setBinBounds(newBounds);
        validate();
    }
    else
    {
        double jumpSize = (m_upBinBoundary - m_lowBinBoundary) / (m_numBins - 2);
        for(int i = 0; i < m_numBins - 1; i++)
        {
            newBounds.append(m_lowBinBoundary + i * jumpSize);
        }
        setBinBounds(newBounds);
        validate();
    }
}

void NumbersBinnerIdea::setNumBins(int numBins)
{
    if (m_numBins == numBins)
        return;

    QJsonObject oldParams = Idea::save();
    oldParams["n"] = m_numBins;

    m_numBins = numBins;
    emit numBinsChanged(m_numBins);

    QJsonObject newParams = Idea::save();
    newParams["n"] = m_numBins;
    reportParamsChanged(oldParams, newParams);
}

double NumbersBinnerIdea::getInitialPositionForDot(int i)
{
    return m_dotInitialPositions[i];
}

QPointF NumbersBinnerIdea::getFinalPositionForDot(int i)
{
    return m_dotFinalPositions[i];
}

double NumbersBinnerIdea::getPositionForBoundLine(int i)
{
    if(i == -1)
        return 0;
    else
        return 450 * (m_binBounds[i] - m_gridBounds[0]) / (m_gridBounds[1] - m_gridBounds[0]);
}

double NumbersBinnerIdea::getBinWidth(int i)
{
    return m_binWidths[i];
}

double NumbersBinnerIdea::getBinHeight(int i)
{
    return m_binHeights[i];
}

void NumbersBinnerIdea::replay()
{
    resetDisplay();
}

void NumbersBinnerIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

void NumbersBinnerIdea::setAutoFitGrid(bool autoFitGrid)
{
    if (m_autoFitGrid == autoFitGrid)
        return;

    QJsonObject oldParams = Idea::save();
    oldParams["a"] = m_autoFitGrid;
    if(autoFitGrid == true)
    { // Turn on autofit. Remember what the old grid looked like.
        QJsonArray gridboundsArray;
        for(int i = 0; i < 4; i++)
            gridboundsArray.append(m_gridBounds.at(i));
        oldParams["gb"] = gridboundsArray;
    }

    m_autoFitGrid = autoFitGrid;
    emit autoFitGridChanged(m_autoFitGrid);

    QJsonObject newParams = Idea::save();
    newParams["a"] = m_autoFitGrid;
    reportParamsChanged(oldParams, newParams);

    if(m_autoFitGrid == true)
        validate();
}

void NumbersBinnerIdea::setLowBinBoundary(double lowBinBoundary)
{
    if (qFuzzyCompare(m_lowBinBoundary, lowBinBoundary))
        return;

    QJsonObject oldParams = Idea::save();
    oldParams["l"] = m_lowBinBoundary;

    m_lowBinBoundary = lowBinBoundary;
    emit lowBinBoundaryChanged(m_lowBinBoundary);

    QJsonObject newParams = Idea::save();
    newParams["l"] = m_lowBinBoundary;
    reportParamsChanged(oldParams, newParams);
}

void NumbersBinnerIdea::setUpBinBoundary(double upBinBoundary)
{
    if (qFuzzyCompare(m_upBinBoundary, upBinBoundary))
        return;

    QJsonObject oldParams = Idea::save();
    oldParams["u"] = m_upBinBoundary;

    m_upBinBoundary = upBinBoundary;
    emit upBinBoundaryChanged(m_upBinBoundary);

    QJsonObject newParams = Idea::save();
    newParams["u"] = m_upBinBoundary;
    reportParamsChanged(oldParams, newParams);
}

void NumbersBinnerIdea::setGridBound(double bound, int index)
{
    //Unchanged
    if(m_gridBounds[index] == bound)
        return;

    // Low less than high etc.
    if(!checkSetGridBound(index, bound))
        return;

    QJsonObject oldParams = Idea::save();
    QJsonArray oldGridBoundsArray;
    for(int i = 0; i < 4; i++)
        oldGridBoundsArray.append(m_gridBounds.at(i));
    oldParams["gb"] = oldGridBoundsArray;
    oldParams["a"] = m_autoFitGrid;
    if(m_autoFitGrid == true)
    {
        m_autoFitGrid = false;
        emit autoFitGridChanged(m_autoFitGrid);
    }

    m_gridBounds[index] = bound;
    emit gridBoundsChanged(m_gridBounds);

    QJsonObject newParams = Idea::save();
    QJsonArray newGridBoundsArray;
    for(int i = 0; i < 4; i++)
        newGridBoundsArray.append(m_gridBounds.at(i));
    newParams["gb"] = newGridBoundsArray;
    newParams["a"] = m_autoFitGrid;

    reportParamsChanged(oldParams, newParams);

    validate();
}

bool NumbersBinnerIdea::checkSetGridBound(int index, double bound)
{
    bool valid = true;

    switch (index)
    {
    case 0:
    {
        if(bound > m_gridBounds[1])
            valid = false;
        break;
    }
    case 1:
    {
        if(bound < m_gridBounds[0])
            valid = false;
        break;
    }
    case 2:
    {
        if(bound > m_gridBounds[3])
            valid = false;
        break;
    }
    case 3:
    {
        if(bound < m_gridBounds[2])
            valid = false;
        break;
    }

    }
    return valid;
}






