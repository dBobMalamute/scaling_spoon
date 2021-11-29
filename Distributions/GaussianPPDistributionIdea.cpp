#include "GaussianPPDistributionIdea.h"
#include <QtMath>
#include <QDebug>
#include <QMatrix4x4>
#include <QJsonArray>

GaussianPPDistributionAttributes::GaussianPPDistributionAttributes()
{
    m_listName = "Gaussian++";
    m_displayName = "Gaussian++";
    m_authors << "dBob";
    m_description = "This box uses the principle of maximum entropy to extend the gaussian. The normal gaussian can be created by inputting a "
                    "mean and standard deviation. Additional distributions can be created by adding an upper bound, a lower bound, or changing"
                    " the skew and kurtosis.";
    m_iconPath = "qrc:/Icons/ModifiableGaussianDistribution.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumberDataTypeAttributes>::cast().id()
                  << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "Low" << "Up" << "Mean" << "Stdev" << "Skew" << "Kurtosis";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/GaussianPPDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Gaussian++";
    m_differentNameOnMinimized = true;

    m_gear = true;
}

const IdeaAttributes GaussianPPDistributionIdea::attributes()
{
    return Magic<GaussianPPDistributionAttributes>::cast();
}

void GaussianPPDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

GaussianPPDistributionIdea::GaussianPPDistributionIdea()
{
    m_options << true << true << true << true << true << true;

    m_params << 0.0 << 0.0 << 0.0 << 0.0 << 0.0 << 0.0;
    m_paramValid << false << false << false << false << false << false;
}

void GaussianPPDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto input = std::dynamic_pointer_cast<NumberDataType>(dataType);
    switch (plugNumber)
    {
    case 0:
        m_lowInput = input;
        break;
    case 1:
        m_upInput = input;
        break;
    case 2:
        m_meanInput = input;
        break;
    case 3:
        m_stdevInput = input;
        break;
    case 4:
        m_skewInput = input;
        break;
    case 5:
        m_kurtosisInput = input;
        break;
    }

    validate();
}

std::shared_ptr<DataType> GaussianPPDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void GaussianPPDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> GaussianPPDistributionIdea::options() const
{
    return m_options;
}

QList<double> GaussianPPDistributionIdea::params() const
{
    return m_params;
}

QList<bool> GaussianPPDistributionIdea::paramValid() const
{
    return m_paramValid;
}

void GaussianPPDistributionIdea::setBound(double value, int index)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setBound(value, index);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);
}

void GaussianPPDistributionIdea::setAutoFit(bool autoFit)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setAutoFit(autoFit);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);
}

void GaussianPPDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void GaussianPPDistributionIdea::setParams(QList<double> params)
{
    if(m_params == params)
        return;

    m_params = params;
    emit paramsChanged(m_params);
}

void GaussianPPDistributionIdea::setParamValid(QList<bool> paramValid)
{
    if(m_paramValid == paramValid)
        return;

    m_paramValid = paramValid;
    emit paramValidChanged(m_paramValid);
}

void GaussianPPDistributionIdea::validate()
{
    //Lock pointers. Memory weirdness reasons.
    auto v_low = m_lowInput.lock();
    auto v_up = m_upInput.lock();
    auto v_mean = m_meanInput.lock();
    auto v_stdev = m_stdevInput.lock();
    auto v_skew = m_skewInput.lock();
    auto v_kurtosis = m_kurtosisInput.lock();

    //Validate parameters. These are the validation steps which can be done regardless
    if(v_low && v_up)
    {
        if(!validateLowUp(v_low->number(),v_up->number()))
            return;
    }
    if(v_low && v_mean)
    {
        if(!validateLowMean(v_low->number(), v_mean->number()))
            return;
    }
    if(v_up && v_mean)
    {
        if(!validateUpMean(v_up->number(),v_mean->number()))
            return;
    }
    if(v_stdev)
    {
        if(!validateStdev(v_stdev->number()))
            return;
    }
    if(v_low && v_up && v_stdev)
    {
        if(!validateStdevLowUp(v_low->number(), v_up->number(), v_stdev->number()))
            return;
    }
    if(v_skew && v_kurtosis)
    {
        if(!validateSkewKurtosis(v_skew->number(),v_kurtosis->number()))
            return;
    }
    if(v_kurtosis)
    {
        if(!validateSkewKurtosis(0.0,v_kurtosis->number()))
            return;
    }

    //Handle the cases with Kurtosis.
    if(v_kurtosis)
    {
        if(v_mean && v_stdev)
        {
            double skew = v_skew ? v_skew->number() : 0.0;
            if(v_up && v_low)
            {
                QString longMessage = "Creating a distribution with a lower bound of " + QString::number(v_low->number()) +
                        " an upper bound of " + QString::number(v_up->number()) +
                        " a mean of " + QString::number(v_mean->number()) +
                        " and a standard deviation of " + QString::number(v_stdev->number()) +
                        " a skew of " + QString::number(skew) + " and a kurtosis of " +
                        QString::number(v_kurtosis->number());
                setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

                QList<double> params;
                params << v_low->number() << v_up->number() << v_mean->number() << v_stdev->number() << skew << v_kurtosis->number();
                QList<bool> paramValid;
                paramValid << true << true << true << true;
                if(v_skew)
                    paramValid << true << true;
                else
                    paramValid << false << true;
                setParams(params);
                setParamValid(paramValid);

                computeSixParameter(v_low->number(), v_up->number(), v_mean->number(), v_stdev->number(), skew, v_kurtosis->number());
            }
            else if(v_up)
            {
                QString longMessage = "Creating a distribution with "
                                      " an upper bound of " + QString::number(v_up->number()) +
                        " a mean of " + QString::number(v_mean->number()) +
                        " and a standard deviation of " + QString::number(v_stdev->number()) +
                        " a skew of " + QString::number(skew) + " and a kurtosis of " +
                        QString::number(v_kurtosis->number());
                setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

                QList<double> params;
                params << 0.0 << v_up->number() << v_mean->number() << v_stdev->number() << skew << v_kurtosis->number();
                QList<bool> paramValid;
                paramValid << false << true << true << true;
                if(v_skew)
                    paramValid << true << true;
                else
                    paramValid << false << true;
                setParams(params);
                setParamValid(paramValid);

                computeSixParameter(v_mean->number() - 15.0*v_stdev->number(), v_up->number(), v_mean->number(), v_stdev->number(), skew, v_kurtosis->number());
            }
            else if(v_low)
            {
                QString longMessage = "Creating a distribution with a lower bound of " + QString::number(v_low->number()) +
                        " a mean of " + QString::number(v_mean->number()) +
                        " and a standard deviation of " + QString::number(v_stdev->number()) +
                        " a skew of " + QString::number(skew) + " and a kurtosis of " +
                        QString::number(v_kurtosis->number());
                setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

                QList<double> params;
                params << v_low->number() << 0.0 << v_mean->number() << v_stdev->number() << skew << v_kurtosis->number();
                QList<bool> paramValid;
                paramValid << true << false << true << true;
                if(v_skew)
                    paramValid << true << true;
                else
                    paramValid << false << true;
                setParams(params);
                setParamValid(paramValid);

                computeSixParameter(v_low->number(), v_mean->number() + 15.0*v_stdev->number(), v_mean->number(), v_stdev->number(), skew, v_kurtosis->number());

            }
            else
            {
                QString longMessage = "Creating a distribution with"
                                      " a mean of " + QString::number(v_mean->number()) +
                        " and a standard deviation of " + QString::number(v_stdev->number()) +
                        " a skew of " + QString::number(skew) + " and a kurtosis of " +
                        QString::number(v_kurtosis->number());
                setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

                QList<double> params;
                params << 0.0 << 0.0 << v_mean->number() << v_stdev->number() << skew << v_kurtosis->number();
                QList<bool> paramValid;
                paramValid << false << false << true << true;
                if(v_skew)
                    paramValid << true << true;
                else
                    paramValid << false << true;
                setParams(params);
                setParamValid(paramValid);

                computeSixParameter(v_mean->number() - 15.0*v_stdev->number(),v_mean->number() + 15.0*v_stdev->number(), v_mean->number(), v_stdev->number(), skew, v_kurtosis->number());

            }
        }
        else
        {
            setStatus(IdeaStatus::Error, "Invalid input", "Adjusting the kurtosis of a distribution without a specified mean and "
                                                          "standard deviation is insufficient input to generate a distribution");
        }
    }
    else if(v_skew)
    {
        if(v_mean && v_stdev)
        {
            double kurtosis = v_kurtosis ? v_kurtosis->number() : 3.0 + 1.5*v_skew->number()*v_skew->number();
            if(v_up && v_low)
            {
                QString longMessage = "Creating a distribution with a lower bound of " + QString::number(v_low->number()) +
                        " an upper bound of " + QString::number(v_up->number()) +
                        " a mean of " + QString::number(v_mean->number()) +
                        " and a standard deviation of " + QString::number(v_stdev->number()) +
                        " a skew of " + QString::number(v_skew->number()) + " and a kurtosis of " +
                        QString::number(kurtosis);
                setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

                QList<double> params;
                params << v_low->number() << v_up->number() << v_mean->number() << v_stdev->number() << v_skew->number() << 0.0;
                QList<bool> paramValid;
                paramValid << true << true << true << true << true << false;
                setParams(params);
                setParamValid(paramValid);

                computeSixParameter(v_low->number(), v_up->number(), v_mean->number(), v_stdev->number(), v_skew->number(), kurtosis);
            }
            else if(v_up)
            {
                QString longMessage = "Creating a distribution with "
                                      " an upper bound of " + QString::number(v_up->number()) +
                        " a mean of " + QString::number(v_mean->number()) +
                        " and a standard deviation of " + QString::number(v_stdev->number()) +
                        " a skew of " + QString::number(v_skew->number()) + " and a kurtosis of " +
                        QString::number(kurtosis);
                setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

                QList<double> params;
                params << 0.0 << v_up->number() << v_mean->number() << v_stdev->number() << v_skew->number() << 0.0;
                QList<bool> paramValid;
                paramValid << false << true << true << true << true << false;
                setParams(params);
                setParamValid(paramValid);

                computeSixParameter(v_mean->number() - 15.0*v_stdev->number(), v_up->number(), v_mean->number(), v_stdev->number(), v_skew->number(), kurtosis);
            }
            else if(v_low)
            {
                QString longMessage = "Creating a distribution with a lower bound of " + QString::number(v_low->number()) +
                        " a mean of " + QString::number(v_mean->number()) +
                        " and a standard deviation of " + QString::number(v_stdev->number()) +
                        " a skew of " + QString::number(v_skew->number()) + " and a kurtosis of " +
                        QString::number(kurtosis);
                setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

                QList<double> params;
                params << v_low->number() << 0.0 << v_mean->number() << v_stdev->number() << v_skew->number() << 0.0;
                QList<bool> paramValid;
                paramValid << true << false << true << true << true << false;
                setParams(params);
                setParamValid(paramValid);

                computeSixParameter(v_low->number(), v_mean->number() + 15.0*v_stdev->number(), v_mean->number(), v_stdev->number(), v_skew->number(), kurtosis);
            }
            else
            {
                QString longMessage = "Creating a distribution with"
                                      " a mean of " + QString::number(v_mean->number()) +
                        " and a standard deviation of " + QString::number(v_stdev->number()) +
                        " a skew of " + QString::number(v_skew->number()) + " and a kurtosis of " +
                        QString::number(kurtosis);
                setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

                QList<double> params;
                params << 0.0 << 0.0 << v_mean->number() << v_stdev->number() << v_skew->number() << 0.0;
                QList<bool> paramValid;
                paramValid << false << false << true << true << true << false;
                setParams(params);
                setParamValid(paramValid);

                computeSixParameter(v_mean->number() - 15.0*v_stdev->number(),v_mean->number() + 15.0*v_stdev->number(), v_mean->number(), v_stdev->number(), v_skew->number(), kurtosis);
            }
        }
        else
        {
            setStatus(IdeaStatus::Error, "Invalid input", "Adjusting the kurtosis of a distribution without a specified mean and "
                                                          "standard deviation is not sufficient information to generate a distribution");
        }
    }
    else if(v_low && v_up && v_mean && v_stdev)
    {
        QString longMessage = "Creating a distribution assuming a lower bound of " + QString::number(v_low->number()) +
                " an upper bound of " + QString::number(v_up->number()) +
                " a mean of " + QString::number(v_mean->number()) +
                " and a standard deviation of " + QString::number(v_stdev->number());
        setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

        QList<double> params;
        params << v_low->number() << v_up->number() << v_mean->number() << v_stdev->number() << 0.0 << 0.0;
        QList<bool> paramValid;
        paramValid << true << true << true << true << false << false;
        setParams(params);
        setParamValid(paramValid);

        computeFourParameter(v_low->number(), v_up->number(), v_mean->number(), v_stdev->number());
        return;
    }
    else if(v_low && v_mean && v_stdev)
    {
        if(!validateLowMeanStdev(v_low->number(), v_mean->number(), v_stdev->number()))
        {
            return;
        }

        QString longMessage = "Creating a distribution assuming a lower bound of " + QString::number(v_low->number()) +
                " a mean of " + QString::number(v_mean->number()) +
                " and a standard deviation of " + QString::number(v_stdev->number());
        setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

        QList<double> params;
        params << v_low->number() << 0.0 << v_mean->number() << v_stdev->number() << 0.0 << 0.0;
        QList<bool> paramValid;
        paramValid << true << false << true << true << false << false;
        setParams(params);
        setParamValid(paramValid);


        computeFourParameter(v_low->number(), v_mean->number() + 15.0* v_stdev->number(), v_mean->number(), v_stdev->number());
        return;
    }
    else if(v_up && v_mean && v_stdev)
    {
        if(!validateUpMeanStdev(v_up->number(), v_mean->number(), v_stdev->number()))
        {
            return;
        }

        QString longMessage = "Creating a distribution assuming "
                              " an upper bound of " + QString::number(v_up->number()) +
                " a mean of " + QString::number(v_mean->number()) +
                " and a standard deviation of " + QString::number(v_stdev->number());
        setStatus(IdeaStatus::Warning, "Might Fail", longMessage);

        QList<double> params;
        params << 0.0 << v_up->number() << v_mean->number() << v_stdev->number() << 0.0 << 0.0;
        QList<bool> paramValid;
        paramValid << false << true << true << true << false << false;
        setParams(params);
        setParamValid(paramValid);


        computeFourParameter(v_mean->number() - 15.0 * v_stdev->number(), v_up->number(), v_mean->number(), v_stdev->number());
        return;
    }
    else if(v_low && v_up && v_mean)
    {
        computeLowUpMean(v_low->number(), v_up->number(), v_mean->number());
        setStatus(IdeaStatus::Error, "Error", "This case doesn't have a name? I can't find one. -dBob");

        QList<double> params;
        params << v_low->number() << v_up->number() << v_mean->number() << 0.0 << 0.0 << 0.0;
        QList<bool> paramValid;
        paramValid << true << true << true << false << false << false;
        setParams(params);
        setParamValid(paramValid);

        return;
    }
    else if(v_low && v_up && v_stdev)
    {
        setStatus(IdeaStatus::Error, "Invalid", "This isn't enough information to create a distribution. Please enter a mean.");
        return;
    }
    else if(v_low && v_mean) // Positive exponential Distribution.
    {
        QString longMessage = "Currently creating a shifted exponential distribution with a mean of "  + QString::number(v_mean->number())
                + " and a lower bound of " + QString::number(v_low->number());
        setStatus(IdeaStatus::Working, "Calculating", longMessage);

        QList<double> params;
        params << v_low->number() << 0.0 << v_mean->number() << 0.0 << 0.0 << 0.0;
        QList<bool> paramValid;
        paramValid << true << false << true << false << false << false;
        setParams(params);
        setParamValid(paramValid);

        computePositiveExponential(v_low->number(),v_mean->number());
        return;
    }
    else if (v_up && v_mean) // Negative Exponential Distribution
    {
        QString longMessage = "Currently creating a shifted and flipped exponential distribution with a mean of "  + QString::number(v_mean->number())
                + " and an upper bound of " + QString::number(v_up->number());
        setStatus(IdeaStatus::Working, "Calculating", longMessage);

        QList<double> params;
        params << 0.0 << v_up->number() << v_mean->number() << 0.0 << 0.0 << 0.0;
        QList<bool> paramValid;
        paramValid << false << true << true << false << false << false;
        setParams(params);
        setParamValid(paramValid);


        computeNegativeExponential(v_up->number(), v_mean->number());
        return;
    }
    else if(v_mean && v_stdev) // Gaussian Distribution.
    {
        QString longMessage = "Currently creating a Gaussian distribution with a mean of "  + QString::number(v_mean->number()) +
                " and a standard deviation of " + QString::number(v_stdev->number());
        setStatus(IdeaStatus::Working, "Calculating", longMessage);

        QList<double> params;
        params << 0.0 << 0.0 << v_mean->number() << v_stdev->number() << 0.0 << 0.0;
        QList<bool> paramValid;
        paramValid << false << false << true << true << false << false;
        setParams(params);
        setParamValid(paramValid);

        computeGaussian(v_mean->number(), v_stdev->number());
        return;
    }
    else if(v_low && v_up) //Uniform Distribution.
    {
        QString longMessage = "Currently creating a Uniform Distribution with a lower bound of "  + QString::number(v_low->number()) +
                " and an upper bound of " + QString::number(v_up->number());
        setStatus(IdeaStatus::Working, "Calculating", longMessage);

        QList<double> params;
        params << v_low->number() << v_up->number() << 0.0 << 0.0 << 0.0 << 0.0;
        QList<bool> paramValid;
        paramValid << true << true << false << false << false << false;
        setParams(params);
        setParamValid(paramValid);

        computeUniform(v_low->number(), v_up->number());
        return;
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

bool GaussianPPDistributionIdea::validateLowUp(double low, double up)
{
    if(low >= up)
    {
        QString longMessage = "The input lower bound " + QString::number(low) + " is greater than the input upper bound " + QString::number(up);
        setStatus(IdeaStatus::Error, "Low >= Up", longMessage);
        m_display->clearData();
        return false;
    }
    return true;
}

bool GaussianPPDistributionIdea::validateLowMean(double low, double mean)
{
    if(low >= mean)
    {
        QString longMessage = "The input lower bound " + QString::number(low) + " is greater than "
                + " the input mean " + QString::number(mean);
        setStatus(IdeaStatus::Error, "Low >= mean", longMessage);
        m_display->clearData();
        return false;
    }
    return true;
}

bool GaussianPPDistributionIdea::validateUpMean(double up, double mean)
{
    if(mean >= up)
    {
        QString longMessage = "The input mean " + QString::number(mean) + " is greater than "
                + " the input upper bound " + QString::number(up);
        setStatus(IdeaStatus::Error, "Mean >= Up", longMessage);
        m_display->clearData();
        return false;
    }
    else
        return true;
}

bool GaussianPPDistributionIdea::validateLowMeanStdev(double low, double mean, double stdev)
{
    if(((mean - low) / stdev) > 1.0)
        return true;
    else
    {
        setStatus(IdeaStatus::Error, "Not Normalizeable", "The input parameters would produce a distribution with"
                                                          " infinite area under the curve. Consider adding an upper "
                                                          "bound or adding skew.");
        m_display->clearData();
        return false;
    }
}

bool GaussianPPDistributionIdea::validateUpMeanStdev(double up, double mean, double stdev)
{
    if(((up - mean) / stdev) > 1.0)
        return true;
    else
    {
        setStatus(IdeaStatus::Error, "Not Normalizeable", "The input parameters would produce a distribution with"
                                                          " infinite area under the curve. Consider adding an upper "
                                                          "bound or adding skew.");
        m_display->clearData();
        return false;
    }
}

bool GaussianPPDistributionIdea::validateStdev(double stdev)
{
    if(stdev <= 0.0)
    {
        QString longMessage = "The input standard deviation " + QString::number(stdev) + " is less than 0. ";
        setStatus(IdeaStatus::Error, "Stdev <= 0", longMessage);
        m_display->clearData();
        return false;
    }
    else
        return true;
}

bool GaussianPPDistributionIdea::validateStdevLowUp(double low, double up, double stdev)
{
    if(stdev >= (up-low) / 2.0)
    {
        QString longMessage = "The input standard deviation " + QString::number(stdev) + " is greater than " +
                "the width available for the distribution [" + QString::number(low) + "-" + QString::number(up) + "]. "
                + "There is no mathematical solution to this.";
        setStatus(IdeaStatus::Error, "Stdev too big", longMessage);
        m_display->clearData();
        return false;
    }
    else
        return true;
}

bool GaussianPPDistributionIdea::validateSkewKurtosis(double skew, double kurtosis)
{
    if(kurtosis < 1.0 + skew*skew)
    {
        QString longMessage = "The inputted kurtosis " + QString::number(kurtosis) + " exceeds 1 plus the square of the skew " +
                QString::number(1.0 + skew*skew) + ". This is mathematically impossible. No distribution will be generated.";
        setStatus(IdeaStatus::Error, "kurtosis too small", longMessage);
        m_display->clearData();
        return false;
    }
        return true;
}

void GaussianPPDistributionIdea::computeSixParameter(double low, double up, double mean, double stdev, double skew, double kurtosis)
{
    emit startSpinningGear();

    //The moments
    double b1 = mean;
    double b2 = stdev*stdev;
    double b3 = skew;
    double b4 = kurtosis;

    //Initial conditions.
    double lambda1 = 0; double lambda2 = 0; double lambda3 = 0; double lambda4 = 0.0;

    int nPoints = 1000; int Niters = 500;
    double dx = (up - low) / (nPoints - 1);

    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(nPoints + 4);
    yValues.reserve(nPoints + 4);

    for(int i = 0; i < nPoints; i++)
    {
        xValues.push_back(low + i*dx);
    }

    for(int j = 0; j < Niters; j++)
    {
        double dQ_dlambda1 = 0.0;
        double dQ_dlambda2 = 0.0;
        double dQ_dlambda3 = 0.0;
        double dQ_dlambda4 = 0.0;

        QMatrix4x4 d2Q_d2(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

        double p_xSum = 0.0;
        for (int i = 0; i < nPoints; i++)
        {
            double b1_term = (xValues[i]-b1);
            double b2_term = (b1_term*b1_term - b2);
            double b3_term = (b1_term*b1_term*b1_term/stdev/stdev/stdev - b3);
            double b4_term = (b1_term*b1_term*b1_term*b1_term/stdev/stdev/stdev/stdev - b4);
            double p_x = dx*qExp(lambda1*b1_term+lambda2*b2_term+lambda3*b3_term+lambda4*b4_term);
            p_xSum += p_x;

            dQ_dlambda1 += p_x*b1_term;
            dQ_dlambda2 += p_x*b2_term;
            dQ_dlambda3 += p_x*b3_term;
            dQ_dlambda4 += p_x*b4_term;

            d2Q_d2(0,0) += p_x * b1_term * b1_term;
            d2Q_d2(1,1) += p_x * b2_term * b2_term;
            d2Q_d2(2,2) += p_x * b3_term * b3_term;
            d2Q_d2(3,3) += p_x * b4_term * b4_term;

            d2Q_d2(0,1) += p_x * b1_term * b2_term;
            d2Q_d2(0,2) += p_x * b1_term * b3_term;
            d2Q_d2(0,3) += p_x * b1_term * b4_term;
            d2Q_d2(1,2) += p_x * b2_term * b3_term;
            d2Q_d2(1,3) += p_x * b2_term * b4_term;
            d2Q_d2(2,3) += p_x * b3_term * b4_term;
        }

        //Fill out symmetric terms.

        d2Q_d2(1,0) = d2Q_d2(0,1);
        d2Q_d2(2,0) = d2Q_d2(0,2);
        d2Q_d2(3,0) = d2Q_d2(0,3);
        d2Q_d2(2,1) = d2Q_d2(1,2);
        d2Q_d2(3,1) = d2Q_d2(1,3);
        d2Q_d2(3,2) = d2Q_d2(2,3);

        QMatrix4x4 d2Q_inv = d2Q_d2.inverted();
        QVector4D dQ(dQ_dlambda1,dQ_dlambda2,dQ_dlambda3,dQ_dlambda4);
        QVector4D deltaMatrix = -0.5*d2Q_inv*dQ;

        lambda1 += deltaMatrix[0];
        lambda2 += deltaMatrix[1];
        lambda3 += deltaMatrix[2];
        lambda4 += deltaMatrix[3];

        if(QVector4D::dotProduct(deltaMatrix, deltaMatrix) < 0.000000000001)
        {
            break;
        }
    }
    double sum = 0.0;

    for (int i = 0; i < nPoints; i++)
    {
        double x_value = low + i*dx;
        double b1_term = (x_value-b1);
        double b2_term = (b1_term*b1_term - b2);
        double b3_term = (b1_term*b1_term*b1_term/stdev/stdev/stdev - b3);
        double b4_term = (b1_term*b1_term*b1_term*b1_term/stdev/stdev/stdev/stdev - b4);
        double y_value = qExp(lambda1*b1_term+lambda2*b2_term+lambda3*b3_term+lambda4*b4_term);
        sum += dx*y_value;
    }
    for (int i = 0; i < nPoints; i++)
    {
        double x_value = low + i*dx;
        double b1_term = (x_value-b1);
        double b2_term = (b1_term*b1_term - b2);
        double b3_term = (b1_term*b1_term*b1_term/stdev/stdev/stdev - b3);
        double b4_term = (b1_term*b1_term*b1_term*b1_term/stdev/stdev/stdev/stdev - b4);
        double y_value = qExp(lambda1*b1_term+lambda2*b2_term+lambda3*b3_term+lambda4*b4_term)/ sum;
        yValues.push_back(y_value);
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

void GaussianPPDistributionIdea::computeFourParameter(double low, double up, double mean, double stdev)
{
    emit startSpinningGear();

    //Calculate the values of the moments.
    double b1 = mean; double b2 = stdev*stdev + b1*b1;

    //Initial conditions.
    double lambda1 = 0; double lambda2 = 0;
    int Npoints = 1000; int Niters = 20;

    double dx = (up - low) / (Npoints - 1);

    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(Npoints + 4);
    yValues.reserve(Npoints + 4);

    for (int i = 0; i < Npoints; i++)
    {
        xValues.push_back(low + i*dx);
    }

    for (int j = 0; j < Niters; j++)
    {
        double dQ_dlambda1 = 0;
        double dQ_dlambda2 = 0;

        double d2Q_dlambda1_2 = 0;
        double d2Q_dlambda1_dlambda2 = 0;
        double d2Q_dlambda2_2 = 0;

        for (int i = 0; i < Npoints; i++)
        {
            double p_x = dx*qExp(lambda1*(xValues[i]-b1)+lambda2*(xValues[i]*xValues[i] - b2));
            dQ_dlambda1 += p_x*(xValues[i]-b1);
            dQ_dlambda2 += p_x*(xValues[i]*xValues[i] - b2);
            d2Q_dlambda1_2 += p_x * (xValues[i]-b1) * (xValues[i]-b1);
            d2Q_dlambda1_dlambda2 += p_x * (xValues[i]-b1) * (xValues[i]*xValues[i] - b2);
            d2Q_dlambda2_2 += p_x * (xValues[i]*xValues[i] - b2) * (xValues[i]*xValues[i] - b2);
        }

        double det = d2Q_dlambda1_2 * d2Q_dlambda2_2 - d2Q_dlambda1_dlambda2 * d2Q_dlambda1_dlambda2;

        double delta_1 = -1.0/det * (d2Q_dlambda2_2 * dQ_dlambda1 - d2Q_dlambda1_dlambda2 * dQ_dlambda2);
        double delta_2 = -1.0/det * (-1.0*d2Q_dlambda1_dlambda2 * dQ_dlambda1 + d2Q_dlambda1_2 * dQ_dlambda2);

        lambda1 += delta_1;
        lambda2 += delta_2;

        if((lambda1* lambda1 + lambda2*lambda2) < 0.000000000001)
            break;
    }


    double sum = 0.0;

    for (int i = 0; i < Npoints; i++)
    {
        double x_value = low + i*dx;
        double y_value = qExp(lambda1*(x_value-b1)+lambda2*(x_value*x_value - b2));
        sum += dx*y_value;
    }
    for (int i = 0; i < Npoints; i++)
    {
        double x_value = low + i*dx;
        double y_value = qExp(lambda1*(x_value-b1)+lambda2*(x_value*x_value - b2)) / sum;
        yValues.push_back(y_value);
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

void GaussianPPDistributionIdea::computeLowUpMean(double low, double up, double mean)
{
    emit startSpinningGear();

    Q_UNUSED(low)
    Q_UNUSED(up)
    Q_UNUSED(mean)
    //This needs more advanced Algorithms. I should probably learn some algorithms.
    // For now, weird.

    emit newData(0);

//    m_display->setXValues(xValues);
//    m_display->setYValues(yValues);
//    m_display->setDistType(DistributionDataType::Continuous);
//    m_display->plot();

    emit stopSpinningGear();
}

void GaussianPPDistributionIdea::computeGaussian(double mean, double stdev)
{
    emit startSpinningGear();
    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(240);
    yValues.reserve(240);

    for(int i = -120; i < 121; i++)
    {
        xValues.push_back(mean + stdev * i / 20.0);
        yValues.push_back(qExp(-1.0 * i * i / 800.0) / (qSqrt(2.0* M_PI)*stdev));
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

void GaussianPPDistributionIdea::computePositiveExponential(double low, double mean)
{
    // I'ma use a little bit of trickery! Calculate a exponential distribution with a mean of:
    // mean - low.
    // Then shift it! -dBob

    emit startSpinningGear();

    double trickMean = mean - low;
    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(302);
    yValues.reserve(302);

    // Calculate the Normalization first.
    double normalization = 0.0;
    for (int i = 0; i < 301; i++)
    {
        normalization += trickMean/20.0*qExp(-1.0*(i+0.0000001)/20);
    }

    xValues.push_back(low);
    yValues.push_back(0.0);

    for (int i = 0; i < 301; i++)
    {
        xValues.push_back(low + trickMean*(i+.0000001)/20.0);
        yValues.push_back(qExp(-1.0*(i+0.5)/20)/normalization);
    }
    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

void GaussianPPDistributionIdea::computeNegativeExponential(double up, double mean)
{
    // I'ma use a little bit of trickery! Calculate a exponential distribution with a mean of:
    // up - mean
    // Then shift and flip it! -dBob

    emit startSpinningGear();

    double trickMean = up - mean;
    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(302);
    yValues.reserve(302);

    // Calculate the Normalization first.
    double normalization = 0.0;
    for (int i = 0; i < 301; i++)
    {
        normalization += trickMean/20.0*qExp(-1.0*(i+0.0000001)/20);
    }

    for (int i = 301; i > -1; i--)
    {
        xValues.push_back(up - trickMean*(i+0.0000001)/20.0);
        yValues.push_back(qExp(-1.0*(i+0.5)/20.0)/normalization);
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

void GaussianPPDistributionIdea::computeUniform(double low, double up)
{
    emit startSpinningGear();

    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(2);
    yValues.reserve(2);

    xValues.push_back(low);
    xValues.push_back(up);

    double length = up - low;
    yValues.push_back(1.0/length);
    yValues.push_back(1.0/length);

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues, yValues);
    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}
QJsonObject GaussianPPDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 6; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void GaussianPPDistributionIdea::load(const QJsonObject &obj)
{
    QJsonValue v = obj["d"];
    if(!v.isUndefined())
    {
        m_display->load(v.toObject());
    }

    v = obj["o"];
    if(!v.isUndefined())
    {
        QJsonArray optionsArray = v.toArray();
        m_options.clear();
        for(int i = 0; i < 6; i++)
        {
            m_options.append(optionsArray.at(i).toBool());
        }
        emit optionsChanged(m_options);
    }
}
