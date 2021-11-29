#include "WireState.h"

WireState::WireState(PlugType plugType) :
    m_requiredPlug(plugType)
{

}

WireState::~WireState()
{

}

void WireState::setRequiredPlug(PlugType end)
{
    m_requiredPlug = end;
}

PlugType WireState::requiredPlug() const
{
    return m_requiredPlug;
}

bool WireState::requiresPlug() const
{
    return m_requiredPlug != PlugType::None;
}

void WireState::setNoRequiredPlug()
{
    m_requiredPlug = PlugType::None;
}

void WireState::resetStopLights()
{
    foreach (CasingBackend* casingBackend, m_casingWithStopLights)
    {
        casingBackend->clearStopLights();
    }
    m_casingWithStopLights.clear();
}

void WireState::casingHasStopLights(CasingBackend *casingBackend)
{
    m_casingWithStopLights.insert(casingBackend);
}
