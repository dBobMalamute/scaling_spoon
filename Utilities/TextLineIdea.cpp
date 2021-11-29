#include "TextLineIdea.h"
#include <QtMath>
#include <QDebug>

TextLineAttributes::TextLineAttributes()
{
    m_listName = "Text Line";
    m_displayName = "";
    m_authors << "dBob";
    m_description = "Allows for displaying a single line of text. Useful for titles. Font size and bold are adjustable by right clicking";

    m_iconPath = "qrc:/Icons/TextLine.png";
    m_embeddedQMLPath = ":/QML/TextLineIdea.qml";
    m_externalQMLPaths << ":/QML/TextControls.qml";
    m_externalQMLLocations.append({true, false, 10,-60});
}

TextLineIdea::TextLineIdea()
{
    setStatus(IdeaStatus::Working, "Showing Text", "Showing Text");

    m_text = "texttext";
    m_bold = true;
    m_fontSize = 14;
}

const IdeaAttributes TextLineIdea::attributes()
{
    return Magic<TextLineAttributes>::cast();
}

QJsonObject TextLineIdea::save() const
{
    QJsonObject saveJson = Idea::save();
    saveJson["text"] = m_text;
    saveJson["bold"] = m_bold;
    saveJson["size"] = m_fontSize;
    return saveJson;
}

void TextLineIdea::load(const QJsonObject &obj)
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

QString TextLineIdea::text() const
{
    return m_text;
}

bool TextLineIdea::bold() const
{
    return m_bold;
}

int TextLineIdea::fontSize() const
{
    return m_fontSize;
}

void TextLineIdea::setText(QString text)
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

void TextLineIdea::setBold(bool bold)
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

void TextLineIdea::setFontSize(int fontSize)
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

