#include "ControlsStyle.h"

ControlsStyle::ControlsStyle(QObject *parent) : QObject(parent)
{
    m_textFont = QFont("consolas", 14, 75);
}

int ControlsStyle::borderWidth() const
{
    return m_borderWidth;
}

QColor ControlsStyle::borderColor() const
{
    return m_borderColor;
}

QColor ControlsStyle::gradientTop() const
{
    return m_gradientTop;
}

QColor ControlsStyle::gradientMid() const
{
    return m_gradientMid;
}

QColor ControlsStyle::gradientBottom() const
{
    return m_gradientBottom;
}

QFont ControlsStyle::textFont() const
{
    return m_textFont;
}

QColor ControlsStyle::textColor() const
{
    return m_textColor;
}

int ControlsStyle::radius() const
{
    return m_radius;
}
