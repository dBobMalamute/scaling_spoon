#include "PostItStyle.h"

PostItStyle::PostItStyle(QObject *parent) : QObject(parent)
{
    m_titleFont = QFont("consolas", 14, 75);
    m_shortMessageFont = QFont("consolas", 12, 75);
    m_longMessageFont = QFont("consolas", 10, 50);
}

QColor PostItStyle::gradientTop() const
{
    return m_gradientTop;
}

QFont PostItStyle::shortMessageFont() const
{
    return m_shortMessageFont;
}

QFont PostItStyle::longMessageFont() const
{
    return m_longMessageFont;
}

QColor PostItStyle::longMessageColor() const
{
    return m_longMessageColor;
}

QColor PostItStyle::shortMessageColor() const
{
    return m_shortMessageColor;
}

QFont PostItStyle::titleFont() const
{
    return m_titleFont;
}

QColor PostItStyle::titleColor() const
{
    return m_titleColor;
}

QColor PostItStyle::glowColor() const
{
    return m_glowColor;
}

double PostItStyle::glowSpread() const
{
    return m_glowSpread;
}

int PostItStyle::glowRadius() const
{
    return m_glowRadius;
}

QColor PostItStyle::borderColorSelected() const
{
    return m_borderColorSelected;
}

QColor PostItStyle::borderColor() const
{
    return m_borderColor;
}

int PostItStyle::borderWidth() const
{
    return m_borderWidth;
}

QColor PostItStyle::gradientBottom() const
{
    return m_gradientBottom;
}
