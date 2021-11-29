#include "StudentTDistributionIdea.h"
#include <QtMath>
#include <QJsonArray>

StudentTDistributionAttributes::StudentTDistributionAttributes()
{
    m_listName = "Student T Distribution";
    m_displayName = "Student T Distribution";
    m_authors << "dBob";
    m_description = "This box creates a student T distribution.";
    m_iconPath = "qrc:/Icons/StudentT.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "DoF";

    m_dataTypesOut << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsOut << "Dist";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/StudentTDistributionIdea.qml";
    m_minimizable = true;
    m_minimizedName = "Student T\nDistribution";
    m_differentNameOnMinimized = true;

    m_gear = true;

}

const IdeaAttributes StudentTDistributionIdea::attributes()
{
    return Magic<StudentTDistributionAttributes>::cast();
}

void StudentTDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

StudentTDistributionIdea::StudentTDistributionIdea()
{
    m_options << true << true << true << true;
}

void StudentTDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    m_inputDoF = std::dynamic_pointer_cast<NumberDataType>(dataType);

    validate();
}

std::shared_ptr<DataType> StudentTDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_distribution);
}

void StudentTDistributionIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}

QList<bool> StudentTDistributionIdea::options() const
{
    return m_options;
}

void StudentTDistributionIdea::setBound(double value, int index)
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

void StudentTDistributionIdea::setAutoFit(bool autoFit)
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

void StudentTDistributionIdea::setOption(bool option, int index)
{
    if (m_options[index] == option)
        return;

    QJsonObject oldParams = save();

    m_options[index] = option;
    emit optionsChanged(m_options);

    QJsonObject newParams = save();
    emit reportParamsChanged(oldParams, newParams);
}

void StudentTDistributionIdea::validate()
{
    auto v_dof = m_inputDoF.lock();

    if(v_dof)
    {
        if(v_dof->number() <= 0.0)
        {
            setStatus(IdeaStatus::Error, "Dof <= 0", "The input degrees of freedom " + QString::number(v_dof->number()) +
                      " is less than 0. ");
            m_display->clearData();
        }
        else
        {
            setStatus(IdeaStatus::Working, "Creating Distribution", "Creating a student-t distribution");
            compute(v_dof->number());
        }
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void StudentTDistributionIdea::compute(double dof)
{
    emit startSpinningGear();
    std::vector<double> xValues;
    std::vector<double> yValues;
    xValues.reserve(240);
    yValues.reserve(240);

    double common = qExp(DistributionCommonFunctions::lnGamma((dof + 1.0) / 2.0) -
                         DistributionCommonFunctions::lnGamma(dof / 2.0)) / qSqrt(dof * M_PI);

    for(int i = -240; i < 241; i++)
    {
        xValues.push_back(i / 20.0);
        yValues.push_back(common * qPow((1 + (i/20.0)*(i/20.0) / dof), -0.5*(dof +1)));
    }

    m_distribution = std::make_shared<DistributionDataType>(DistributionDataType::Continuous, xValues,yValues);
    if(dof > 1.0)
        m_distribution->setMean(0.0);
    else
        m_distribution->setFlags(MEANUNDEFINED);
    m_distribution->setMedian(0.0);
    m_distribution->setMode(0.0);

    if(dof > 2.0)
        m_distribution->setVariance(dof / (dof - 2.0));
    else if (dof > 1.0)
        m_distribution->setVariance(Q_INFINITY);
    else
        m_distribution->setFlags(VARIANCEUNDEFINED);

    if(dof > 3)
        m_distribution->setSkew(0);
    else
        m_distribution->setFlags(SKEWUNDEFINED);

    if(dof > 4.0)
        m_distribution->setKurtosis(3.0 + 6.0 / (dof - 4.0));
    else if (dof > 2.0)
        m_distribution->setKurtosis(Q_INFINITY);
    else
        m_distribution->setFlags(KURTOSISUNDEFINED);



    emit newData(0);

    m_display->setXValues(xValues);
    m_display->setYValues(yValues);
    m_display->setDistType(DistributionDataType::Continuous);
    m_display->plot();

    emit stopSpinningGear();
}

QJsonObject StudentTDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["d"] = m_display->save();

    QJsonArray optionsArray;
    for(int i = 0; i < 4; i++)
        optionsArray.append(m_options.at(i));
    saveJson["o"] = optionsArray;

    return saveJson;
}

void StudentTDistributionIdea::load(const QJsonObject &obj)
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
        for(int i = 0; i < 4; i++)
        {
            m_options.append(optionsArray.at(i).toBool());
        }
        emit optionsChanged(m_options);
    }
}
