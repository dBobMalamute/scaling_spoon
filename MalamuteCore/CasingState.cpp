#include "CasingState.h"

#include "InheritedClasses/Idea.h"
#include "WireBackend.h"
#include <QDebug>

CasingState::CasingState(std::unique_ptr<Idea> const &idea)
    : m_inWires(idea->attributes().dataTypesIn().size())
    , m_outWires(idea->attributes().dataTypesOut().size())
{

}


std::vector<CasingState::IdBackendSet> const &CasingState::getEntries(PlugType plugType) const
{
    if (plugType == PlugType::DataIn)
        return m_inWires;
    else
        return m_outWires;
}

std::vector<CasingState::IdBackendSet> &CasingState::getEntries(PlugType plugType)
{
    if (plugType == PlugType::DataIn)
    {
        return m_inWires;
    }
    else
        return m_outWires;
}

CasingState::IdBackendSet CasingState::wires(PlugType plugType, PlugNumber plugNumber) const
{
    auto const &wires = getEntries(plugType);

    return wires[static_cast<unsigned int>(plugNumber)];
}

std::vector<CasingState::IdBackendSet> CasingState::wires(PlugType plugType) const
{
    if(plugType == PlugType::DataIn)
        return m_inWires;
    else
        return m_outWires;
}

QVector<QUuid> CasingState::wireIds(PlugType plugType)
{
    QVector<QUuid> ids;

    if(plugType == PlugType::DataIn)
    {
        for(unsigned long i = 0; i < m_inWires.size(); i++)
        {
            for(auto c : m_inWires[i])
            {
                ids.append(c.first);
            }
        }
    }
    else
    {
        for(unsigned long i = 0; i < m_outWires.size(); i++)
        {
            for(auto c : m_outWires[i])
            {
                ids.append(c.first);
            }
        }
    }
    return ids;
}

void CasingState::setWire(PlugType plugType, PlugNumber plugNumber, WireBackend& wireBackend)
{
    auto &wires = getEntries(plugType);
    wires.at(static_cast<unsigned int>(plugNumber)).insert(std::make_pair(wireBackend.id(), &wireBackend));
}

void CasingState::eraseWire(PlugType plugType, PlugNumber plugNumber, QUuid id)
{
    getEntries(plugType)[static_cast<unsigned int>(plugNumber)].erase(id);
}

void CasingState::clearStopLights()
{
    m_stopLights.clear();
}

void CasingState::addStopLight(PlugType plugType, PlugNumber plugNumber, QColor color)
{
    m_stopLights.push_back(StopLight(plugType,plugNumber,color));
}
