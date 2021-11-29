#include "NumberDisplayIdea.h"

NumberDisplayAttributes::NumberDisplayAttributes()
{
    m_listName = "Number Display";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "Displays a single number";
    m_iconPath = "qrc:/Icons/NumberDisplay.png";

    m_dataTypesIn << Magic<NumberDataTypeAttributes>::cast().id();
    m_labelsIn << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/NumberDisplayIdea.qml";
}

NumberDisplayIdea::NumberDisplayIdea()
{

}

const IdeaAttributes NumberDisplayIdea::attributes()
{
    return Magic<NumberDisplayAttributes>::cast();
}

std::shared_ptr<DataType> NumberDisplayIdea::dataOut(PlugNumber)
{
    return nullptr;
}

void NumberDisplayIdea::onNewDataIn(std::shared_ptr<DataType> data, int)
{
    auto numberData = std::dynamic_pointer_cast<NumberDataType>(data);

    if (numberData)
    {
        setStatus(IdeaStatus::Working, "Displaying", "Currently displaying the number " + QString::number(numberData->number()));
        emit displayNumber(QString::number(numberData->number()));
    }
    else
    {
        setDefaultStatus();
        clearDisplay();
    }
}


