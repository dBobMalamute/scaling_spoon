#include "NumbersInputIdea.h"
#include <QDebug>

NumbersInputAttributes::NumbersInputAttributes()
{
    m_listName = "Numbers Input";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "This box allows the user to enter a list of numbers.";
    m_iconPath = "qrc:/Icons/NumbersInput.png";

    m_dataTypesOut << Magic<NumbersDataTypeAttributes>::cast().id();
    m_labelsOut << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/NumbersInputIdea.qml";
}

const IdeaAttributes NumbersInputIdea::attributes()
{
    return Magic<NumbersInputAttributes>::cast();
}

std::shared_ptr<DataType> NumbersInputIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_nums);
}

NumbersInputIdea::NumbersInputIdea()
{
    QList<double> tempList;
    tempList << 1.0 << 2.0 << 3.0;
    m_nums = std::make_shared<NumbersDataType>(tempList);
    setStatus(IdeaStatus::Working,"Sourcing","Sourcing 1.0, 2.0, 3.0");
}

void NumbersInputIdea::setNums(QString nums)
{
    QStringList numsList = nums.simplified().split(",");
    numsList.removeAll(QString(""));
    numsList.removeAll(QString(" "));
    QList<double> numsVector;

    bool ok = false;
    for(int i = 0; i < numsList.length(); i++)
    {

        double numVal = numsList[i].simplified().toDouble(&ok);
        if(ok)
        {
            numsVector.append(numVal);
        }
        else
        {
            break;
        }

    }
    if(!ok)
    {
        setStatus(IdeaStatus::Error, "Invalid Input", "Could not parse the input numbers. Please enter your numbers sep"
                                                      "arated by commas");
        return;
    }

    if(numsVector == m_nums->numbers())
        return;

    //Record the change for the undo/redo framework
    QJsonObject oldParams = this->save();
    m_nums = std::make_shared<NumbersDataType>(numsVector);

    QJsonObject newParams = this->save();
    emit reportParamsChanged(oldParams,newParams);

    setStatus(IdeaStatus::Working, "Creating bins", "The idea is currently sourcing the numbers: " + m_nums->toString());
    emit newData(0);

}

QString NumbersInputIdea::getNums()
{
    return m_nums->toString();
}


QJsonObject NumbersInputIdea::save() const
{
    QJsonObject modelJson = Idea::save();
    modelJson["nums"] = m_nums->toString();

    return modelJson;
}

void NumbersInputIdea::load(const QJsonObject &p)
{
    QJsonValue v = p["nums"];

    if (!v.isUndefined())
    {
        QString numsString = v.toString();
        numsString = numsString.simplified();
        QStringList numsStringList = numsString.split(",");
        QList<double> numsVector;
        for (int i = 0; i < numsStringList.size(); i++)
        {
            numsVector.append(numsStringList[i].simplified().toDouble());
        }
        m_nums = std::make_shared<NumbersDataType>(numsVector);
        emit numsChanged();
        emit newData(0);
    }
}


