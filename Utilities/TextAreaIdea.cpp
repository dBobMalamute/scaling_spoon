#include "TextAreaIdea.h"
#include <QtMath>
#include <QDebug>

TextAreaAttributes::TextAreaAttributes()
{
    m_listName = "Text Area";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "Creates an area where text can be entered. Font size is adjustable by right clicking.";

    m_iconPath = "qrc:/Icons/TextArea.png";
    m_embeddedQMLPath = ":/QML/TextAreaIdea.qml";
    m_externalQMLPaths << ":/QML/TextControls.qml";
    m_externalQMLLocations.append({true, false, 10,-60});

    m_minimizable = true;
    m_startMinimized = false;
}

TextAreaIdea::TextAreaIdea()
{
    setStatus(IdeaStatus::Working, "Showing Text", "Showing Text");

    m_text = "text\ntext";
    m_fontSize = 10;
    m_bold = true;
}

const IdeaAttributes TextAreaIdea::attributes()
{
    return Magic<TextAreaAttributes>::cast();
}

QJsonObject TextAreaIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["text"] = m_text;
    saveJson["size"] = m_fontSize;
    saveJson["bold"] = m_bold;
    return saveJson;
}

void TextAreaIdea::load(const QJsonObject &obj)
{
    QJsonValue txt = obj["text"];
    if(!txt.isUndefined())
    {
        m_text = txt.toString();
        emit textChanged();
    }

    QJsonValue bld = obj["bold"];
    if(!bld.isUndefined())
    {
        m_bold = bld.toBool();
        emit boldChanged(m_bold);
    }

    QJsonValue sze = obj["size"];
    if(!sze.isUndefined())
    {
        m_fontSize = sze.toInt();
        emit fontSizeChanged(m_fontSize);
    }
}

QString TextAreaIdea::text() const
{
    return m_text;
}

int TextAreaIdea::fontSize() const
{
    return m_fontSize;
}

bool TextAreaIdea::bold() const
{
    return m_bold;
}

void TextAreaIdea::setText(QString text)
{
    if (m_text == text)
        return;
    else
    {
        QJsonObject oldParams = this->save();
        m_text = text;
        emit textChanged();

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);
    }
}

void TextAreaIdea::setFontSize(int fontSize)
{
    if (m_fontSize == fontSize)
        return;
    else
    {
        QJsonObject oldParams = this->save();
        m_fontSize = fontSize;
        emit fontSizeChanged(m_fontSize);

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);
    }
}

void TextAreaIdea::setBold(bool bold)
{
    if (m_bold == bold)
        return;
    else
    {
        QJsonObject oldParams = this->save();
        m_bold = bold;
        emit boldChanged(m_bold);

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);
    }
}

