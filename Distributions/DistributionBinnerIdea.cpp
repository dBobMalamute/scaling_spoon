#include "DistributionBinnerIdea.h"
#include <QDebug>

#include <QJsonArray>

DistributionBinnerAttributes::DistributionBinnerAttributes()
{
    m_listName = "Distribution Binner";
    m_displayName = "Distribution Binner";
    m_authors << "dBob";
    m_description = "This box turns a list of numbers into a bar-style chart";
    m_iconPath = "qrc:/Icons/DistributionBinner.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsIn << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/DistributionBinnerIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Binner";
    m_differentNameOnMinimized = true;

    m_externalQMLPaths << ":/QML/DistributionBinnerControls.qml"
                       << ":/QML/DistributionBinnerControls2.qml";
    m_externalQMLLocations.append({true, false, 10,0});
    m_externalQMLLocations.append({true, false, 10,120});

    m_gear = true;
}

const IdeaAttributes DistributionBinnerIdea::attributes()
{
    return Magic<DistributionBinnerAttributes>::cast();
}

DistributionBinnerIdea::DistributionBinnerIdea() :
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

QJsonObject DistributionBinnerIdea::save() const
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

void DistributionBinnerIdea::load(const QJsonObject &obj)
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

void DistributionBinnerIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    m_dist = std::dynamic_pointer_cast<DistributionDataType>(dataType);

    validate();
}

void DistributionBinnerIdea::resetDisplay()
{
    emit binBoundsChanged(m_binBounds);

    if(status() == IdeaStatus::Working || status() == IdeaStatus::DisplayError)
        emit plotContinuous(m_xValues, m_yValues, m_finalYValues);
}

void DistributionBinnerIdea::validate()
{
    auto v_dist = m_dist.lock();
    if(v_dist)
    {
        if(m_binBounds.length() == 0)
        {
            setStatus(IdeaStatus::StandBy, "Enter Bounds", "Please enter the bounds you wish to turn the input set of "
                                                           "numbers into.");
            emit plotContinuous(std::vector<double>(), std::vector<double>(), std::vector<double>());
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
                emit plotContinuous(std::vector<double>(), std::vector<double>(), std::vector<double>());
            }
            else
            {
                setStatus(IdeaStatus::Working, "Binning Distribution", "Currently binning the input distribution");

                if(v_dist->distType() == DistributionDataType::Continuous)
                {
                    std::vector<double> tempXValues = v_dist->xValues();
                    std::vector<double> tempYValues = v_dist->yValues();

                    m_xValues.clear();
                    m_yValues.clear();
                    m_xValues.push_back(tempXValues[0]);
                    m_yValues.push_back(0.0);
                    for(size_t i = 0; i < tempXValues.size(); i++)
                    {
                        m_xValues.push_back(tempXValues[i]);
                        m_yValues.push_back(tempYValues[i]);
                    }
                    m_xValues.push_back(tempXValues[tempXValues.size() - 1]);
                    m_yValues.push_back(0.0);
                    computeContinuous();
                }
                else
                {
                    m_xValues = v_dist->xValues();
                    m_yValues = v_dist->yValues();
                    computeDiscrete();
                }
            }
        }
    }
    else
    {
        setDefaultStatus();
        emit plotContinuous(std::vector<double>(), std::vector<double>(), std::vector<double>());
    }
}

void DistributionBinnerIdea::computeBinWidths()
{
    m_binWidths.clear();

    m_binWidths.reserve(m_binBounds.size() + 1);
    m_binWidths.append(450 * (m_binBounds[0] - m_gridBounds[0]) / (m_gridBounds[1] - m_gridBounds[0]));
    for(int i = 1; i < m_binBounds.size(); i++)
    {
        m_binWidths.append((int) 450 *(m_binBounds[i] - m_binBounds[i-1]) / (m_gridBounds[1] - m_gridBounds[0]));
    }
    m_binWidths.append(450 * (m_gridBounds[1] - m_binBounds[m_binBounds.size() - 1]) / (m_gridBounds[1] - m_gridBounds[0]));
}

void DistributionBinnerIdea::computeContinuous()
{    
    emit startSpinningGear();

    //Clear old data;
    m_finalYValues.clear();

    //Create a combined Trellis of x-values, y_values. For bin boundaries use -1 as a sentinal value.
    size_t iter_points = 0; int iter_bounds = 0;
    std::vector<double> interpXValues;
    std::vector<double> interpYValues;
    QList<int> binNumber;
    interpXValues.reserve(2*m_binBounds.length() + m_xValues.size() + 2);
    interpYValues.reserve(2*m_binBounds.length() + m_xValues.size() + 2);
    binNumber.reserve(2*m_binBounds.length() + m_xValues.size() + 2);

    double xValue = qMin(m_binBounds[0], m_xValues[0]) - 0.00000001;
    double yValue = 0.0;
    interpXValues.push_back(xValue);
    interpYValues.push_back(yValue);
    binNumber.append(0);

    //Add a fake bin bound to make 2 of the upcoming loop cleaner.
    m_binBounds.append(qMax(m_binBounds[m_binBounds.length() - 1], m_xValues[m_xValues.size() - 1]) + 0.00000001);

    while (iter_points < m_xValues.size())
    {
        if(m_xValues[iter_points] < m_binBounds[iter_bounds])
        {
            // Add a point from the distribution.
            interpXValues.push_back(m_xValues[iter_points]);
            interpYValues.push_back(m_yValues[iter_points]);
            binNumber.append(iter_bounds);
            iter_points++;
        }
        else
        {
            interpXValues.push_back(m_binBounds[iter_bounds] - 0.00000001);
            interpYValues.push_back(-1.0);
            binNumber.append(iter_bounds);

            interpXValues.push_back(m_binBounds[iter_bounds] + 0.00000001);
            interpYValues.push_back(-1.0);
            iter_bounds++;
            binNumber.append(iter_bounds);

        }
    }

    //Handles case where the entire distribution is well to the left of the bins.
    while(iter_bounds < m_binBounds.length() - 1)
    {
        interpXValues.push_back(m_binBounds[iter_bounds] - 0.00000001);
        interpYValues.push_back(-1.0);
        binNumber.append(iter_bounds);

        interpXValues.push_back(m_binBounds[iter_bounds] + 0.00000001);
        interpYValues.push_back(-1.0);
        iter_bounds++;
        binNumber.append(iter_bounds);
    }

    // There are a few times where the points in x-values are not in order.
    // Example is when the distribution points and the bin boundaries have overlaps.
    // This ensures the x-values are in order. I don't bother with the y-values because
    // they are close enough to correct.
    std::sort(interpXValues.begin(), interpXValues.end());


    //Add a zero value at the end. Helps with an edge case.
    interpXValues.push_back(qMax(m_binBounds[m_binBounds.length()-1], m_xValues[m_xValues.size()-1] + 0.0000001));
    interpYValues.push_back(0.0);
    binNumber.append(iter_bounds);

    //Extrapolate values for the new combined distribution for the pounts which were bin boundaries.

    //Replace the -1 values with linearly interpolated points.
    for(size_t i = 1; i < interpXValues.size()-1; i++)
    {
        if(interpYValues[i] < -0.5)
        {
            int lower = i-1;
            int higher = i+1;
            while(interpYValues[lower] < -0.5)
                lower--;
            while(interpYValues[higher] < -0.5)
                higher++;
            double x_val = interpXValues[i];
            double x_val_higher = interpXValues[higher];
            double x_val_lower = interpXValues[lower];
            double y_val_higher = interpYValues[higher];
            double y_val_lower = interpYValues[lower];
            double y_val = (x_val_higher-x_val)/(x_val_higher-x_val_lower)* y_val_lower;
            y_val += (x_val - x_val_lower)/(x_val_higher-x_val_lower)* y_val_higher;
            interpYValues[i] = y_val;
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

    if(m_autoFitGrid)
    {
        m_gridBounds[0] = interpXValues[0];
        m_gridBounds[1] = interpXValues[interpXValues.size() - 1];
    }
    else
    {

        if(m_binBounds[0] < m_gridBounds[0] || m_binBounds[m_binBounds.size() - 2] > m_gridBounds[1])
            setStatus(IdeaStatus::DisplayError, "Display Error?", "The bounds set might not contain all the data."
                                                                  " This can be misleading. Consider turning autofit on.");
    }

    //Prepare binWidths, place to put the first dot, etc
    computeBinWidths();

    //Iterate through the combined list and find the new bins.
    int bin_iter = 0;
    for(size_t i = 1; i < interpXValues.size(); i++)
    {
        if(interpXValues[i] < m_binBounds[bin_iter] + 0.0000001)
        {
            binHeights[bin_iter] += (interpXValues[i] - interpXValues[i-1])*0.5*
                    (interpYValues[i] + interpYValues[i-1]);        }
        else
        {
            i--;
            bin_iter++;
        }
    }

    //Earlier I added a fake bin bound to make loops cleaner. This removes it.
    m_binBounds.removeLast();

    //Calculate the normalization constant.
    double sum = 0.0;
    for (int i = 0; i < binHeights.size(); i++)
    {
        sum += binHeights[i];
    }

    //Normalize. Also find the highest bin for the display.
    double height = 0.0;
    if (sum >= 0.2)
    {
        for (int i = 0; i < binHeights.size(); i++)
        {
            binHeights[i] /= sum;
            if(binHeights[i] > height)
                height = binHeights[i];
        }
    }
    else
    {
        qFatal("Binning failed. Not normalizeable distribution?");
    }

    //Calculate the final heights for each of the points of the distribution.
    for(int i = 0; i < binNumber.length() - 1; i++)
    {
        m_finalYValues.push_back(binHeights[binNumber[i]]);
    }
    m_finalYValues.push_back(0.0);

    //Find the highest point of the yValues;
    double max = interpYValues[0];
    for(size_t i = 1; i < interpYValues.size(); i++)
    {
        if(max < interpYValues[i])
            max = interpYValues[i];
    }

    // Makes the distribution fill up most of the screen.
    std::vector<double> relativeYValues;
    relativeYValues.reserve(interpYValues.size());
    for(size_t i = 0; i < interpYValues.size(); i++)
    {
        relativeYValues.push_back(interpYValues[i] / max * 0.98);
    }

    //Fit the grid bounds to make it look nice.
    if(m_autoFitGrid)
    {
        m_gridBounds[2] = 0;
        m_gridBounds[3] = 1.1 * height;
        emit gridBoundsChanged(m_gridBounds);
    }
    else
    {
        if(m_gridBounds[2] > 0 || m_gridBounds[3] < height)
            setStatus(IdeaStatus::DisplayError,"Display Error?", "The grid bounds might not contain all the data. "
                                                                 "This can be misleading, consider turning autofit on.");
    }

    // Plot the thing
    m_binHeights = binHeights;
    m_xValues = interpXValues;
    m_yValues = relativeYValues;

    emit plotContinuous(m_xValues, m_yValues, m_finalYValues);

    emit stopSpinningGear();
}

void DistributionBinnerIdea::computeDiscrete()
{
    emit startSpinningGear();

    if(m_autoFitGrid)
    {
        double visualLowBound = qMin(m_binBounds[0], m_xValues[0]);
        double visualUpBound = qMax(m_binBounds.last(), m_xValues[m_xValues.size() - 1]);

        m_gridBounds[0] = visualLowBound;
        m_gridBounds[1] = visualUpBound;
    }

    //Prepare binWidths, place to put the first dot, etc
    computeBinWidths();

    m_binHeights.clear();
    for(int i = 0; i < m_numBins; i++)
    {
        m_binHeights.append(0.0);
    }

    //Add a fake bin bound to make 2 of the upcoming loop cleaner.
    m_binBounds.append(qMax(m_binBounds[m_binBounds.length() - 1], m_xValues[m_xValues.size() - 1]) + 0.00000001);

    size_t iter_points = 0; int iter_bounds = 0;

    while(iter_points < m_xValues.size())
    {
        if(m_xValues[iter_points] < m_binBounds[iter_bounds])
        {
            m_binHeights[iter_bounds] += m_yValues[iter_points];
            iter_points++;
        }
        else
        {
            iter_bounds++;
        }
    }
    //Earlier I added a fake bin bound to make loops cleaner. This removes it.
    m_binBounds.removeLast();

    //Calculate the normalization constant.
    double sum = 0.0;
    for (int i = 0; i < m_binHeights.size(); i++)
    {
        sum += m_binHeights[i];
    }

    //Normalize. Also find the highest bin for the display.
    double height = 0.0;
    if (sum >= 0.2)
    {
        for (int i = 0; i < m_binHeights.size(); i++)
        {
            m_binHeights[i] /= sum;
            if(m_binHeights[i] > height)
                height = m_binHeights[i];
        }
    }
    else
    {
        qFatal("Binning failed. Not normalizeable distribution?");
    }

    //Fit the grid bounds to make it look nice.
    if(m_autoFitGrid)
    {
        m_gridBounds[2] = 0;
        m_gridBounds[3] = 1.025 * height;
        emit gridBoundsChanged(m_gridBounds);
    }
    else
    {
        if(m_gridBounds[2] > 0 || m_gridBounds[3] < height)
            setStatus(IdeaStatus::DisplayError,"Display Error?", "The grid bounds might not contain all the data. "
                                                                 "This can be misleading, consider turning autofit on.");
    }

    emit plotDiscrete(m_xValues, m_yValues);
    emit stopSpinningGear();
}

QList<double> DistributionBinnerIdea::binBounds() const
{
    return m_binBounds;
}

int DistributionBinnerIdea::numBins() const
{
    return m_numBins;
}

double DistributionBinnerIdea::lowBinBoundary() const
{
    return m_lowBinBoundary;
}

double DistributionBinnerIdea::upBinBoundary() const
{
    return m_upBinBoundary;
}

double DistributionBinnerIdea::animationSpeed() const
{
    return m_animationSpeed;
}

bool DistributionBinnerIdea::autoFitGrid() const
{
    return m_autoFitGrid;
}

QList<double> DistributionBinnerIdea::gridBounds() const
{
    return m_gridBounds;
}

void DistributionBinnerIdea::setBinBounds(QList<double> bounds)
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

void DistributionBinnerIdea::sanitizeInputBounds(QString input)
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

void DistributionBinnerIdea::setBoundsByButton()
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

void DistributionBinnerIdea::setNumBins(int numBins)
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

double DistributionBinnerIdea::getPositionForBoundLine(int i)
{
    if(i == -1)
        return 0;
    else
        return 450 * (m_binBounds[i] - m_gridBounds[0]) / (m_gridBounds[1] - m_gridBounds[0]);
}

double DistributionBinnerIdea::getBinWidth(int i)
{
    return m_binWidths[i];
}

double DistributionBinnerIdea::getBinHeight(int i)
{
    return m_binHeights[i];
}

void DistributionBinnerIdea::replay()
{
    resetDisplay();
}

void DistributionBinnerIdea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

void DistributionBinnerIdea::setLowBinBoundary(double lowBinBoundary)
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

void DistributionBinnerIdea::setUpBinBoundary(double upBinBoundary)
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

void DistributionBinnerIdea::setAutoFitGrid(bool autoFitGrid)
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

void DistributionBinnerIdea::setGridBound(double bound, int index)
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

bool DistributionBinnerIdea::checkSetGridBound(int index, double bound)
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
