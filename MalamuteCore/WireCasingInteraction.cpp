#include "WireCasingInteraction.h"

#include "Wire.h"
#include "Casing.h"
#include "InheritedClasses/Idea.h"
#include "CasingBackend.h"

WireCasingInteraction::WireCasingInteraction(CasingBackend &casingBackend, WireBackend& wireBackend)
    : m_casingBackend(&casingBackend)
    , m_wireBackend(&wireBackend)
{}

bool WireCasingInteraction::canConnect(PlugNumber &plugNumber) const
{
    PlugType requiredPlug = wireRequiredPlug();
    if (requiredPlug == PlugType::None)
    {
        return false;
    }

    CasingBackend* casingBackend = m_wireBackend->getCasing(oppositePlug(requiredPlug));
    if (casingBackend == m_casingBackend)
        return false;

    QPointF wirePoint = wireEndScenePosition(requiredPlug);
    plugNumber = casingPlugNumberUnderScenePoint(requiredPlug, wirePoint);
    if (plugNumber == INVALID)
    {
        return false;
    }

    if (!casingPlugIsEmpty(requiredPlug, plugNumber))
        return false;

    int wireDataType = m_wireBackend->dataType(oppositePlug(requiredPlug));
    auto const &idea = m_casingBackend->idea();

    int candidateCasingDataType;
    if(requiredPlug == PlugType::DataIn)
        candidateCasingDataType = idea->attributes().dataTypesIn()[plugNumber];
    else
        candidateCasingDataType = idea->attributes().dataTypesOut()[plugNumber];

    if(candidateCasingDataType != wireDataType)
        return false;

    return true;
}


bool WireCasingInteraction::tryConnect() const
{
    PlugNumber plugNumber = INVALID;

    if (!canConnect(plugNumber))
    {
        return false;
    }

    PlugType requiredPlug = wireRequiredPlug();

    m_casingBackend->casingState().setWire(requiredPlug,plugNumber,*m_wireBackend);
    m_wireBackend->setCasingToPlug(m_casingBackend, requiredPlug, plugNumber);

    PlugNumber inPlugNumber = m_wireBackend->getPlugIndex(PlugType::DataIn);
    PlugNumber outPlugNumber = m_wireBackend->getPlugIndex(PlugType::DataOut);
    auto inCasingBackend = m_wireBackend->getCasing(PlugType::DataIn);
    auto outCasingBackend = m_wireBackend->getCasing(PlugType::DataOut);

    inCasingBackend->casingState().setWire(PlugType::DataIn, inPlugNumber, *m_wireBackend);
    outCasingBackend->casingState().setWire(PlugType::DataOut, outPlugNumber, *m_wireBackend);

    outCasingBackend->sendDataNewWire(m_wireBackend->id());

    return true;
}

bool WireCasingInteraction::disconnect(PlugType disconnectingPlug) const
{
    PlugNumber plugNumber = m_wireBackend->getPlugIndex(disconnectingPlug);

    CasingState &state = m_casingBackend->casingState();

    state.getEntries(disconnectingPlug)[plugNumber].clear();

    m_wireBackend->clearCasingBackend(disconnectingPlug);
    m_wireBackend->setRequiredPlugType(disconnectingPlug);
    m_wireBackend->getWire()->grabMouse();

    return true;
}

bool WireCasingInteraction::tryHover()
{
    PlugNumber plugNumber = INVALID;

    PlugType requiredPlug = wireRequiredPlug();
    if (requiredPlug == PlugType::None)
    {
        return false;
    }

    //Things cannot connect to themselves. This checks that. No Recursion!
    CasingBackend* casingBackend = m_wireBackend->getCasing(oppositePlug(requiredPlug));
    if (casingBackend == m_casingBackend)
        return false;

    //Find the plug number. else, return out of this.
    QPointF wirePoint = wireEndScenePosition(requiredPlug);
    plugNumber = casingPlugNumberUnderScenePoint(requiredPlug, wirePoint);
    if (plugNumber == INVALID)
    {
        return false;
    }

    //Cannot connect to a plug that already has a wire. Red Circle!
    if (!casingPlugIsEmpty(requiredPlug, plugNumber))
    {
        m_casingBackend->addRedLight(requiredPlug,plugNumber);
        return false;
    }

    //Wrong data type.
    int wireDataType = m_wireBackend->dataType(oppositePlug(requiredPlug));
    auto const &idea = m_casingBackend->idea();

    int candidateCasingDataType;
    if(requiredPlug == PlugType::DataIn)
        candidateCasingDataType = idea->attributes().dataTypesIn()[plugNumber];
    else
        candidateCasingDataType = idea->attributes().dataTypesOut()[plugNumber];

    if(candidateCasingDataType != wireDataType)
    {
        m_casingBackend->addRedLight(requiredPlug,plugNumber);
        return false;
    }

    //Correct data type. Draw a green circle.

    m_casingBackend->addGreenLight(requiredPlug,plugNumber);
    return true;
}

// ------------------ util functions below

PlugType WireCasingInteraction::wireRequiredPlug() const
{
    auto const &state = m_wireBackend->state();
    return state.requiredPlug();
}

QPointF WireCasingInteraction::wireEndScenePosition(PlugType plugType) const
{
    auto wire = m_wireBackend->getWire();

    QPointF wirePos = wire->getEndPoint(plugType);
    QPointF corkboardPos = QPointF(wirePos.x() + wire->x(), wirePos.y() + wire->y());

    return corkboardPos;
}

PlugNumber WireCasingInteraction::casingPlugNumberUnderScenePoint(PlugType plugType,
                                                                        QPointF const & scenePoint) const
{
    double x_shift = m_casingBackend->casing().x();
    double y_shift = m_casingBackend->casing().y();
    QPointF casingPos = QPointF(scenePoint.x()-x_shift,scenePoint.y()-y_shift);
    PlugNumber plugNumber = m_casingBackend->casing().checkHitScenePointHighTolerance(plugType, casingPos);

    return plugNumber;
}

bool WireCasingInteraction::casingPlugIsEmpty(PlugType plugType, PlugNumber plugNumber) const
{
    CasingState const & state = m_casingBackend->casingState();
    auto const & entries = state.getEntries(plugType);

    if (entries[plugNumber].empty())
        return true;

    return ( plugType == PlugType::DataOut);
}
