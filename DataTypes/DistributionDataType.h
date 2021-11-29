#ifndef DISTRIBUTIONDATATYPES_H
#define DISTRIBUTIONDATATYPES_H

#include <QVariant>
#include <QString>
#include <QColor>
#include <QDebug>

#include "../MalamuteCore/InheritedClasses/DataType.h"

#define MEANSET 1
#define MEANUNDEFINED 2
#define MEDIANSET 4
#define MEDIANUNDEFINED 16
#define MODESET 32
#define MODEUNDEFINED 64
#define VARIANCESET 128
#define VARIANCEUNDEFINED 256
#define SKEWSET 512
#define SKEWUNDEFINED 1024
#define KURTOSISSET 2048
#define KURTOSISUNDEFINED 4096

struct DistributionDataTypeAttributes : public DataTypeAttributes
{
    DistributionDataTypeAttributes();
};

class DistributionDataType : public DataType
{
public:

     enum DistributionType
     {
         Continuous,
         Discrete
     };

    DistributionDataType(DistributionType distType, std::vector<double> xValues, std::vector<double> yValues);

    DataTypeAttributes getAttributes() override;
    std::vector<double> xValues() const;
    std::vector<double> yValues() const;
    DistributionType distType() const;

    long long getFlags() const;
    void setFlags(long long newFlags);

    double getMean() const;
    void setMean(double mean);

    double getMedian() const;
    void setMedian(double median);

    double getMode() const;
    void setMode(double mode);

    double getVariance() const;
    void setVariance(double variance);

    double getSkew() const;
    void setSkew(double skew);

    double getKurtosis() const;
    void setKurtosis(double kurtosis);

private:
    std::vector<double> m_xValues;
    std::vector<double> m_yValues;
    DistributionType m_distType;

    long long flags;
    double m_mean;
    double m_median;
    double m_mode;
    double m_variance;
    double m_skew;
    double m_kurtosis;
};

#endif
