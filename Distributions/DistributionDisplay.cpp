#include "DistributionDisplay.h"

DistributionDisplay::DistributionDisplay(QObject *parent) : QObject(parent),
    m_xLow(-1.0),
    m_xHigh(1.0),
    m_yLow(0.0),
    m_yHigh(1.0),
    m_autoFit(true)
{

}

bool DistributionDisplay::setBound(double value, int index)
{
    bool ok = false;
    if(index == 0 && !qFuzzyCompare(value, m_xLow) && value < m_xHigh)
    {
        m_xLow = value;
        emit xLowChanged(m_xLow);
        ok = true;
    }
    else if(index == 1 && !qFuzzyCompare(value, m_xHigh) && value > m_xLow)
    {
        m_xHigh = value;
        emit xHighChanged(m_xHigh);
        ok = true;
    }
    else if(index == 2 && !qFuzzyCompare(value, m_yLow) && value < m_yHigh)
    {
        m_yLow = value;
        emit yLowChanged(m_yLow);
        ok = true;
    }
    else if(index == 3 && !qFuzzyCompare(value, m_yHigh) && value > m_yLow)
    {
        m_yHigh = value;
        emit yHighChanged(m_yHigh);
        ok = true;
    }

    if(ok && m_autoFit)
    {
        m_autoFit = false;
        emit autoFitChanged(m_autoFit);
    }
    plot();
    return ok;
}

bool DistributionDisplay::setAutoFit(bool autoFit)
{
    if (m_autoFit == autoFit)
        return false;

    m_autoFit = autoFit;
    emit autoFitChanged(m_autoFit);

    if(m_autoFit)
        plot();
    return true;
}

double DistributionDisplay::xLow() const
{
    return m_xLow;
}

double DistributionDisplay::xHigh() const
{
    return m_xHigh;
}

double DistributionDisplay::yLow() const
{
    return m_yLow;
}

double DistributionDisplay::yHigh() const
{
    return m_yHigh;
}

bool DistributionDisplay::autoFit() const
{
    return m_autoFit;
}

QJsonObject DistributionDisplay::save() const
{
    QJsonObject saveJson;
    saveJson["xl"] = xLow();
    saveJson["xh"] = xHigh();
    saveJson["yl"] = yLow();
    saveJson["yh"] = yHigh();
    saveJson["a"] = autoFit();

    return saveJson;
}

void DistributionDisplay::load(const QJsonObject &obj)
{
    QJsonValue v;

    v = obj["xl"];
    if(!v.isUndefined())
    {
        m_xLow = v.toDouble();
        emit xLowChanged(m_xLow);
    }
    v = obj["xh"];
    if(!v.isUndefined())
    {
        m_xHigh = v.toDouble();
        emit xHighChanged(m_xHigh);
    }
    v = obj["yl"];
    if(!v.isUndefined())
    {
        m_yLow = v.toDouble();
        emit yLowChanged(m_yLow);
    }
    v = obj["yh"];
    if(!v.isUndefined())
    {
        m_yHigh = v.toDouble();
        emit yHighChanged(m_yHigh);
    }

    v = obj["a"];
    if(!v.isUndefined())
    {
        m_autoFit = v.toBool();
        emit autoFitChanged(m_autoFit);
    }

    if(m_autoFit && m_xValues.size() != 0)
        fit();

    plot();
}

void DistributionDisplay::setXLow(double xLow)
{
    if (qFuzzyCompare(m_xLow, xLow))
        return;

    m_xLow = xLow;
    emit xLowChanged(m_xLow);
}

void DistributionDisplay::setXHigh(double xHigh)
{
    if (qFuzzyCompare(m_xHigh, xHigh))
        return;

    m_xHigh = xHigh;
    emit xHighChanged(m_xHigh);
}

void DistributionDisplay::setYLow(double yLow)
{
    if (qFuzzyCompare(m_yLow, yLow))
        return;

    m_yLow = yLow;
    emit yLowChanged(m_yLow);
}

void DistributionDisplay::setYHigh(double yHigh)
{
    if (qFuzzyCompare(m_yHigh, yHigh))
        return;

    m_yHigh = yHigh;
    emit yHighChanged(m_yHigh);
}

void DistributionDisplay::fit()
{
    if(m_distType == DistributionDataType::Discrete)
    {
        if(qFuzzyIsNull(m_xValues[0]))
            setXLow(0.0);
        else
            setXLow(m_xValues[0]);

        if(qFuzzyIsNull(m_xValues[m_xValues.size() - 1]))
            setXHigh(1.0);
        else
            setXHigh(m_xValues[m_xValues.size() - 1] + 1.0);
    }
    else
    {
        if(qFuzzyIsNull(m_xValues[0]))
            setXLow(0.0);
        else
            setXLow(m_xValues[0]);

        if(qFuzzyIsNull(m_xValues[m_xValues.size() - 1]))
            setXHigh(0.0);
        else
            setXHigh(m_xValues[m_xValues.size() - 1]);
    }

    double yMin = 0.0;
    double yMax = m_yValues[0];
    for(size_t i = 1; i < m_yValues.size(); i++)
    {
        if(yMax < m_yValues[i])
            yMax = m_yValues[i];
    }
    double yLength = yMax - yMin;
    setYLow(yMin);
    setYHigh(yMax + yLength / 20.0);
}

void DistributionDisplay::plot()
{
    //Plot can be called before data is available. This handles the edge case.
    if(m_xValues.size() == 0)
        return;

    if(m_autoFit)
        fit();

    if(m_distType == DistributionDataType::Continuous)
        plotContinuous();
    else
        plotDiscrete();
}

void DistributionDisplay::plotContinuous()
{
    double xLength = m_xHigh - m_xLow;
    double yLength = m_yHigh - m_yLow;

    std::vector<double> xPos;
    std::vector<double> yPos;
    xPos.reserve(m_xValues.size() + 3);
    yPos.reserve(m_xValues.size() + 3);

    xPos.push_back(450.0 * (m_xValues[0] - m_xLow) / xLength);
    yPos.push_back(301.0);
    for(size_t i = 0; i < m_xValues.size(); i++)
    {
        xPos.push_back(450.0 * (m_xValues[i] - m_xLow) / xLength);
        yPos.push_back(300.0 - (300.0 * (m_yValues[i] - m_yLow) / yLength));
    }

    xPos.push_back(450.0 * (m_xValues[m_xValues.size() - 1]  - m_xLow) / xLength);
    yPos.push_back(301.0);
    xPos.push_back(450.0 * (m_xValues[0] - m_xLow) / xLength);
    yPos.push_back(301.0);

    emit displayContinuousDistribution(xPos, yPos);
}

void DistributionDisplay::plotDiscrete()
{
    double xLength = m_xHigh - m_xLow;
    double yLength = m_yHigh - m_yLow;

    std::vector<double> xPos;
    std::vector<double> yPos;
    xPos.reserve(m_xValues.size());
    yPos.reserve(m_xValues.size());

    for(size_t i = 0; i < m_xValues.size(); i++)
    {
        xPos.push_back(450.0 * (m_xValues[i] - m_xLow) / xLength);
        yPos.push_back(300.0 - (300.0 * (m_yValues[i] - m_yLow) / yLength));
    }

    emit displayDiscreteDistribution(xPos, yPos);
}

void DistributionDisplay::setDistType(const DistributionDataType::DistributionType &distType)
{
    m_distType = distType;
}

void DistributionDisplay::clearData()
{
    m_xValues.clear();
    m_yValues.clear();
}

void DistributionDisplay::setYValues(const std::vector<double> &yValues)
{
    m_yValues = yValues;
}

void DistributionDisplay::setXValues(const std::vector<double> &xValues)
{
    m_xValues = xValues;
}


