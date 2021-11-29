#include "DistributionDisplayIdea.h"
#include <QJsonArray>

#include "DistributionCommonFunctions.h"

DistributionDisplayAttributes::DistributionDisplayAttributes()
{
    m_listName = "Distribution Display";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This box displays a distribution.";
    m_iconPath = "qrc:/Icons/DistributionDisplay.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/DistributionDisplayIdea.qml";
}

const IdeaAttributes DistributionDisplayIdea::attributes()
{
    return Magic<DistributionDisplayAttributes>::cast();
}

DistributionDisplayIdea::DistributionDisplayIdea()
{
}

void DistributionDisplayIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

void DistributionDisplayIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto distribution = std::dynamic_pointer_cast<DistributionDataType>(dataType);
    m_distribution = distribution;

    validate();
}

QJsonObject DistributionDisplayIdea::save() const
{
    QJsonObject saveJson = Idea::save();

    saveJson["d"] = m_display->save();

    return saveJson;
}

void DistributionDisplayIdea::load(const QJsonObject &obj)
{
    QJsonValue v = obj["d"];
    if(!v.isUndefined())
    {
        m_display->load(v.toObject());
    }
}

void DistributionDisplayIdea::validate()
{
    auto v_distribution = m_distribution.lock();
    if(v_distribution)
    {
        setStatus(IdeaStatus::Working, "Plotting", "This idea box is currently plotting the input distribution.");
        m_display->setXValues(v_distribution->xValues());
        m_display->setYValues(v_distribution->yValues());
        m_display->setDistType(v_distribution->distType());

        m_display->plot();
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void DistributionDisplayIdea::setBound(double value, int index)
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

void DistributionDisplayIdea::setAutoFit(bool autoFit)
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
