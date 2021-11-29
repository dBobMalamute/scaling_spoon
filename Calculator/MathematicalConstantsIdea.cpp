#include "MathematicalConstantsIdea.h"

#include <QJsonValue>
#include <QtMath>

MathematicalConstantsAttributes::MathematicalConstantsAttributes()
{
    m_listName = "Mathematical Constants";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This idea allows the user to conveniently enter some of the more common numbers in mathematics.";
    m_iconPath = "qrc:/Icons/MathematicalConstants.png";

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/MathematicalConstantsIdea.qml";
}

MathematicalConstantsIdea::MathematicalConstantsIdea()
{
    m_number = std::make_shared<NumberDataType>(M_PI);
    setStatus(IdeaStatus::Working, "Sourcing: PI", "");
}

const IdeaAttributes MathematicalConstantsIdea::attributes()
{
    return Magic<MathematicalConstantsAttributes>::cast();
}

QJsonObject MathematicalConstantsIdea::save() const
{
    QJsonObject modelJson = Idea::save();
    modelJson["i"] = m_index;

    return modelJson;
}

void MathematicalConstantsIdea::load(QJsonObject const &p)
{
    QJsonValue v = p["i"];

    if (!v.isUndefined())
    {
        m_index = v.toInt();
        emit indexChanged(m_index);
        setNumber();
    }
}

std::shared_ptr<DataType> MathematicalConstantsIdea::dataOut(PlugNumber)
{
    return m_number;
}

void MathematicalConstantsIdea::setIndex(int i)
{
    if(m_index == i)
        return;

    QJsonObject oldParams = this->save();

    m_index = i;
    emit indexChanged(m_index);

    QJsonObject newParams = this->save();
    emit reportParamsChanged(oldParams, newParams);

    setNumber();
}

int MathematicalConstantsIdea::getIndex()
{
    return m_index;
}

void MathematicalConstantsIdea::setNumber()
{
    switch (m_index)
    {
    case 0:
        m_number = std::make_shared<NumberDataType>(M_PI);
        emit newData(0);
        break;
    case 1:
        m_number = std::make_shared<NumberDataType>(M_E);
        emit newData(0);
        break;
    case 2:
        m_number = std::make_shared<NumberDataType>(M_PI_2);
        emit newData(0);
        break;

    case 3:
        m_number = std::make_shared<NumberDataType>(M_PI_4);
        emit newData(0);
        break;

    case 4:
        m_number = std::make_shared<NumberDataType>(M_1_PI);
        emit newData(0);
        break;

    case 5:
        m_number = std::make_shared<NumberDataType>(1.0 / M_E);
        emit newData(0);
        break;
    }
    setStatus(IdeaStatus::Working, "Sourcing", "Sourcing: " + QString::number(m_number->number()));
}
