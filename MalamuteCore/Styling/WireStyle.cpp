#include "WireStyle.h"

WireStyle::WireStyle(QObject *parent) : QObject(parent)
{

}

double WireStyle::lineWidth()
{
    return m_lineWidth;
}

double WireStyle::endpointSize()
{
    return m_endpointSize;
}

QColor WireStyle::selectedColor()
{
    return m_selectedColor;
}

double WireStyle::selectedLineWidth()
{
    return m_selectedLineWidth;
}

double WireStyle::transmittingLineWidth()
{
    return m_transmittingLineWidth;
}

double WireStyle::transmittingLightenFactor()
{
    return m_transmittingLightenFactor;
}

int WireStyle::transmittingAnimationTime()
{
    return m_transmittingAnimationTime;
}
