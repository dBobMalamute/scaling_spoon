#include "GearStyle.h"

GearStyle::GearStyle(QObject *parent) : QObject(parent)
{

}

QString GearStyle::gearPath() const
{
    return m_gearPath;
}

int GearStyle::gearSpeed() const
{
    return m_gearSpeed;
}
