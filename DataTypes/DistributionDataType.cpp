#include "DistributionDataType.h"

DistributionDataTypeAttributes::DistributionDataTypeAttributes()
{
    m_name = "Continuous Distribution";
    m_color = "#095bb5";
    m_abbreviation = "Dist";
    m_description = "This data types holds a continuous distribution represented as a two dimensional set of numbers.";
    m_authors << "dBob";

    m_defaultDisplay = "Distribution Display";
    m_defaultSource = "Gaussian Distribution";
}

DataTypeAttributes DistributionDataType::getAttributes()
{
    return Magic<DistributionDataTypeAttributes>::cast();
}

DistributionDataType::DistributionDataType(DistributionType distType, std::vector<double> xValues, std::vector<double> yValues) :
    flags(0)
{
    m_distType = distType;
    m_xValues = xValues;
    m_yValues = yValues;
}

std::vector<double> DistributionDataType::xValues() const
{
    return m_xValues;
}

std::vector<double> DistributionDataType::yValues() const
{
    return m_yValues;
}

DistributionDataType::DistributionType DistributionDataType::distType() const
{
    return m_distType;
}

long long DistributionDataType::getFlags() const
{
    return flags;
}

void DistributionDataType::setFlags(long long newFlags)
{
    flags |= newFlags;
}

double DistributionDataType::getMean() const
{
    return m_mean;
}

void DistributionDataType::setMean(double mean)
{
    flags |= MEANSET;
    m_mean = mean;
}

double DistributionDataType::getMedian() const
{
    return m_median;
}

void DistributionDataType::setMedian(double median)
{
    flags |= MEDIANSET;
    m_median = median;
}

double DistributionDataType::getMode() const
{
    return m_mode;
}

void DistributionDataType::setMode(double mode)
{
    flags |= MODESET;
    m_mode = mode;
}

double DistributionDataType::getVariance() const
{
    return m_variance;
}

void DistributionDataType::setVariance(double variance)
{
    flags |= VARIANCESET;
    m_variance = variance;
}

double DistributionDataType::getSkew() const
{
    return m_skew;
}

void DistributionDataType::setSkew(double skew)
{
    flags |= SKEWSET;
    m_skew = skew;
}

double DistributionDataType::getKurtosis() const
{
    return m_kurtosis;
}

void DistributionDataType::setKurtosis(double kurtosis)
{
    flags |= KURTOSISSET;
    m_kurtosis = kurtosis;
}


