#include "ColoredSquareIdea.h"
#include <QtMath>
#include <QDebug>
#include <QQmlEngine>

ColoredSquareAttributes::ColoredSquareAttributes()
{
    m_listName = "Colored Square";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "Test Idea please ignore.";

    m_iconPath = "";
    m_embeddedQMLPath = ":/QML/ColoredSquare.qml";

    m_resizeable = true;

    qmlRegisterType(QUrl("qrc:/QML/LoadTestingRectangle.qml"),"com.testing.test", 1, 0, "LoadTestingRectangle");
}

ColoredSquareIdea::ColoredSquareIdea()
{
    setStatus(IdeaStatus::Working, "Showing Square", "Showing Square");

}

const IdeaAttributes ColoredSquareIdea::attributes()
{
    return Magic<ColoredSquareAttributes>::cast();
}

QJsonObject ColoredSquareIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    return saveJson;
}

void ColoredSquareIdea::load(const QJsonObject &)
{

}


