#include "CorkboardStyle.h"

CorkboardStyle::CorkboardStyle(QObject *parent) : QObject(parent)
{

}

QColor CorkboardStyle::selectionRectangleColor() const
{
    return m_selectionRectangleColor;
}

int CorkboardStyle::selectionRectangleWidth() const
{
    return m_selectionRectangleWidth;
}

int CorkboardStyle::selectionRectangleRadius() const
{
    return m_selectionRectangleRadius;
}

QString CorkboardStyle::backgroundImagePath() const
{
    return m_backgroundImagePath;
}

QColor CorkboardStyle::backgroundTintColor() const
{
    return m_backgroundTintColor;
}
