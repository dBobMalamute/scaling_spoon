#ifndef CASINGSTATE_H
#define CASINGSTATE_H

#include <vector>
#include <unordered_map>
#include <memory>

#include <QUuid>
#include <QColor>
#include "Plug.h"
#include "InheritedClasses/DataType.h"

class WireBackend;
class Idea;

struct StopLight
{
    StopLight(PlugType plugType, PlugNumber plugNumber, QColor color)
    {
        m_plugType = plugType;
        m_plugNumber = plugNumber;
        m_color = color;
    }
    PlugType m_plugType;
    PlugNumber m_plugNumber;
    QColor m_color;
};

namespace std
{
template<>struct hash<QUuid>
{
    inline
    std::size_t
    operator()(QUuid const& uid) const
    {
        return qHash(uid);
    }
};
}

/// Contains vectors of connected input and output wires.
/// Stores bool for reacting on hovering wires
///
class CasingState
{
public:
    CasingState(std::unique_ptr<Idea> const &idea);

public:
    using IdBackendSet = std::unordered_map<QUuid, WireBackend*>;

    /// Returns vector of wire IDs.
    /// Some of them can be empty (null)
    std::vector<IdBackendSet> const& getEntries(PlugType) const;
    std::vector<IdBackendSet> & getEntries(PlugType);

    IdBackendSet wires(PlugType plugType, PlugNumber plugNumber) const;
    std::vector<IdBackendSet> wires(PlugType plugType) const;
    QVector<QUuid> wireIds(PlugType plugType);

    void setWire(PlugType plugType, PlugNumber plugNumber, WireBackend& wireBackend);
    void eraseWire(PlugType plugType, PlugNumber plugNumber, QUuid id);

    void clearStopLights();
    void addStopLight(PlugType plugType, PlugNumber plugNumber, QColor color);

private:
    std::vector<IdBackendSet> m_inWires;
    std::vector<IdBackendSet> m_outWires;

    std::vector<StopLight> m_stopLights;
};

#endif
