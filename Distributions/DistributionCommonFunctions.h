#ifndef DISTRIBUTIONCOMMONFUNCTIONS_H
#define DISTRIBUTIONCOMMONFUNCTIONS_H

#include "../DataTypes/DistributionDataType.h"
#include <QtMath>

class DistributionCommonFunctions
{
public:
    static std::vector<double> makeCDF(DistributionDataType::DistributionType type,
                                       std::vector<double> xValues, std::vector<double> yValues);
    static std::vector<double> makeContinuousCDF(std::vector<double> xValues, std::vector<double> yValues);
    static std::vector<double> makeDiscreteCDF(std::vector<double> xValues, std::vector<double> yValues);

    static double lnGamma(double v);
};

#endif // DISTRIBUTIONCOMMONFUNCTIONS_H
