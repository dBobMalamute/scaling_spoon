#include "LabeledNumberDisplayIdea.h"

LabeledNumberDisplayAttributes::LabeledNumberDisplayAttributes()
{
    m_listName = "Labeled Number Display";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "Shows a number which can be labeled";
    m_iconPath = "qrc:/Icons/LabeledNumberDisplay.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/LabeledNumberDisplayIdea.qml";
}

LabeledNumberDisplayIdea::LabeledNumberDisplayIdea()
{
    m_text = "Label";
}

LabeledNumberDisplayIdea::~LabeledNumberDisplayIdea() {}

const IdeaAttributes LabeledNumberDisplayIdea::attributes()
{
    return Magic<LabeledNumberDisplayAttributes>::cast();
}

QJsonObject LabeledNumberDisplayIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["txt"] = m_text;

    return saveJson;
}

void LabeledNumberDisplayIdea::load(const QJsonObject &p)
{
    QJsonValue v2 = p["txt"];

    if (!v2.isUndefined())
    {
        m_text = v2.toString();
        emit textChanged();
    }
}

std::shared_ptr<DataType> LabeledNumberDisplayIdea::dataOut(PlugNumber)
{
    return nullptr;
}

void LabeledNumberDisplayIdea::onNewDataIn(std::shared_ptr<DataType> data, int)
{
    auto numberData = std::dynamic_pointer_cast<NumberDataType>(data);

    if (numberData)
    {
        setStatus(IdeaStatus::Working, "Displaying", "Currently displaying the number " + QString::number(numberData->number()));
        emit displayNumber(QString::number(numberData->number()));
    }
    else
    {
        emit displayNumber("0.0");
        setDefaultStatus();
    }
}

QString LabeledNumberDisplayIdea::text() const
{
    return m_text;
}

void LabeledNumberDisplayIdea::setText(QString text)
{
    if (m_text == text)
        return;
    else
    {
        QJsonObject oldParams = this->save();
        m_text = text;
        emit textChanged();

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams, newParams);
    }
}


