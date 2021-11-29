#include "CasingStyle.h"

CasingStyle::CasingStyle(QObject *parent) : QObject(parent)
{
    m_gradientTopColors << QColor("#338347") << QColor("#6b8333") << QColor("#33836c")
                           << QColor("#337b83") << QColor("#5b3383") << QColor("#836a33");

    m_gradientBottomColors << QColor("#71A67B") << QColor("#96a671") << QColor("#71a696")
                           << QColor("#71a0a6") << QColor("#8b71a6") << QColor("#a69571");

    m_nameFont = QFont("consolas", 12,75);
    m_portLabelsFont = QFont("consolas", 11, 50);
    m_validationTextFont = QFont("consolas", 12,75);
}

double CasingStyle::bordersWidth() const
{
    return m_bordersWidth;
}

QColor CasingStyle::borderColor() const
{
    return m_borderColor;
}

QColor CasingStyle::borderSelectedColor() const
{
    return m_borderSelectedColor;
}

QColor CasingStyle::gradientTopColor(int i)
{
    return m_gradientTopColors.at(i);
}

QColor CasingStyle::gradientBottomColor(int i)
{
    return m_gradientBottomColors.at(i);
}

int CasingStyle::glowRadius() const
{
    return m_glowRadius;
}

double CasingStyle::glowSpread() const
{
    return m_glowSpread;
}

QColor CasingStyle::glowColor() const
{
    return m_glowColor;
}

double CasingStyle::portSize() const
{
    return m_portSize;
}

double CasingStyle::portSpacing() const
{
    return m_portSpacing;
}

double CasingStyle::portBorderWidth() const
{
    return m_portBorderWidth;
}

QColor CasingStyle::nameColor() const
{
    return m_nameColor;
}

QFont CasingStyle::nameFont() const
{
    return m_nameFont;
}

QColor CasingStyle::portLabelsColor() const
{
    return m_portLabelsColor;
}

QFont CasingStyle::portLabelsFont() const
{
    return m_portLabelsFont;
}

QColor CasingStyle::validationTextColor() const
{
    return m_validationTextColor;
}

QFont CasingStyle::validationTextFont() const
{
    return m_validationTextFont;
}

double CasingStyle::buttonSize() const
{
    return m_buttonSize;
}

QColor CasingStyle::buttonColor() const
{
    return m_buttonColor;
}

double CasingStyle::buttonHoverDarkenFactor() const
{
    return m_buttonHoverDarkenFactor;
}

QColor CasingStyle::resizerColor() const
{
    return m_resizerColor;
}

QColor CasingStyle::resizerBorderColor() const
{
    return m_resizerBorderColor;
}

double CasingStyle::frameRadius() const
{
    return m_frameRadius;
}

double CasingStyle::resizerSize() const
{
    return m_resizerSize;
}
