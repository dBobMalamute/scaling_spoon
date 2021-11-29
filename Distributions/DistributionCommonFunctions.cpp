#include "DistributionCommonFunctions.h"
#include <QDebug>

std::vector<double> DistributionCommonFunctions::makeCDF(DistributionDataType::DistributionType type, std::vector<double> xValues, std::vector<double> yValues)
{
    if(type == DistributionDataType::Continuous)
        return makeContinuousCDF(xValues, yValues);
    else
        return makeDiscreteCDF(xValues, yValues);
}

std::vector<double> DistributionCommonFunctions::makeContinuousCDF(std::vector<double> xValues, std::vector<double> yValues)
{
    //Prepare the output.
    std::vector<double> cdf;
    cdf.reserve(xValues.size());

    //Trapezoids.
    double runningSum = 0.0;
    cdf.push_back(runningSum);
    for(size_t i = 0; i < xValues.size() -1; i++)
    {
        runningSum += 0.5*(yValues[i+1] + yValues[i]) * (xValues[i+1] - xValues[i]);
        cdf.push_back(runningSum);
    }

    //Normalize. In an ideal world this wouldn't be needed. In practice, it's occasionally off by too much.
    double norm = cdf[cdf.size() - 1];
    for(size_t i = 0; i < cdf.size(); i++)
    {
        cdf[i] /= norm;
    }
    return cdf;
}

std::vector<double> DistributionCommonFunctions::makeDiscreteCDF(std::vector<double> xValues, std::vector<double> yValues)
{
    //Prepare the output.
    std::vector<double> cdf;
    cdf.reserve(xValues.size());

    //Sum up the values.
    double runningSum = 0.0;
    for(size_t i = 0; i < xValues.size() - 1; i++)
    {
        runningSum += yValues[i];
        cdf.push_back(runningSum);
    }

    //Normalize. In an ideal world this wouldn't be needed. In practice, it's occasionally off by too much.
    double norm = cdf[cdf.size() - 1];
    for(size_t i = 0; i < cdf.size(); i++)
    {
        cdf[i] /= norm;
    }

    return cdf;
}

double DistributionCommonFunctions::lnGamma(double v)
{
    // Lanczos C. 1964. A precision approximation of the gamma function.
    double temp; double y; double ser;

    static const double cof[14] = {57.1562356658629235, -59.5979603554754912,
                                  14.1360979747417471, -0.491913816097620199, .339946499848118887e-4,
                                  .465236289270485756e-4, -.983744753048795646e-4, .158088703224912494e-3,
                                  -.210264441724104883e-3, .21743961811521264e-3, -.164318106536762890e-3,
                                  .844182239838527433e-4, -.261908384015814087e-4, .368991826595316234e-5};
    y = v;
    temp = v + 5.2421875;
    temp = (0.5 + v) * qLn(temp) - temp;
    ser = 0.99999999999999702;
    for(int i = 0; i < 14; i++)
        ser += cof[i] / (y + i + 1);

    return temp + qLn(2.5066282746310005 * ser / v);
}
