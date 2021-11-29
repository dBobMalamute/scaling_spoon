#include "NumbersDisplayIdea.h"
#include <QDebug>

NumbersDisplayAttributes::NumbersDisplayAttributes()
{
    m_listName = "Numbers Display";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This box displays a list of numbers.";
    m_iconPath = "qrc:/Icons/NumbersDisplay.png";

    m_dataTypesIn << Magic<NumbersDataTypeAttributes>::cast().id();
    m_labelsIn << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/NumbersDisplayIdea.qml";
}

const IdeaAttributes NumbersDisplayIdea::attributes()
{
    return Magic<NumbersDisplayAttributes>::cast();
}

NumbersDisplayIdea::NumbersDisplayIdea()
{

}

void NumbersDisplayIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto numsData = std::dynamic_pointer_cast<NumbersDataType>(dataType);
    if(numsData)
    {
        setStatus(IdeaStatus::Working, "Displaying", "Currently displaying the numbers: " + numsData->toString());
        emit displayNums(numsData->toString());
    }
    else
    {
        emit clearNums();
        setDefaultStatus();
    }
}

