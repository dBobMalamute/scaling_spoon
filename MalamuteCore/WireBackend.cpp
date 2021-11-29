#include "WireBackend.h"

WireBackend::WireBackend(PlugType plugType, CasingBackend* casingBackend, PlugNumber plugNumber)
    : m_uid(QUuid::createUuid())
    , m_outPlugNumber(INVALID)
    , m_inPlugNumber(INVALID)
    , m_state()
{
    setCasingToPlug(casingBackend, plugType, plugNumber);
    setRequiredPlugType(oppositePlug(plugType));
}


WireBackend::WireBackend(QUuid id, CasingBackend* inCasingBackend, PlugNumber inPlugNumber,
                         CasingBackend* outCasingBackend, PlugNumber outPlugNumber)
    : m_uid(id)
    , m_outCasingBackend(outCasingBackend)
    , m_inCasingBackend(inCasingBackend)
    , m_outPlugNumber(outPlugNumber)
    , m_inPlugNumber(inPlugNumber)
    , m_state()
{
    setCasingToPlug(inCasingBackend, PlugType::DataIn, inPlugNumber);
    setCasingToPlug(outCasingBackend, PlugType::DataOut, outPlugNumber);
}


WireBackend::~WireBackend()
{
    if (complete())
    {
        wireBackendMadeIncomplete(*this);
        propagateEmptyData();
    }
    delete m_wire;
}


QJsonObject WireBackend::serialize() const
{
    QJsonObject wireJson;

    if (m_inCasingBackend && m_outCasingBackend)
    {
        wireJson["id"] = id().toString();
        wireJson["i_id"] = m_inCasingBackend->id().toString();
        wireJson["i_n"] = m_inPlugNumber;

        wireJson["o_id"] = m_outCasingBackend->id().toString();
        wireJson["o_n"] = m_outPlugNumber;
    }
    return wireJson;
}


QUuid WireBackend::id() const
{
    return m_uid;
}


bool WireBackend::complete() const
{
    return m_inCasingBackend != nullptr && m_outCasingBackend != nullptr;
}


void WireBackend::setRequiredPlugType(PlugType plugType)
{
    m_state.setRequiredPlug(plugType);

    switch (plugType)
    {
    case PlugType::DataOut:
        m_outCasingBackend      = nullptr;
        m_outPlugNumber = INVALID;
        break;

    case PlugType::DataIn:
        m_inCasingBackend      = nullptr;
        m_inPlugNumber = INVALID;
        break;

    default:
        break;
    }
}


PlugType WireBackend::requiredPlugType() const
{
    return m_state.requiredPlug();
}


void WireBackend::setWire(Wire *wire)
{
    m_wire = wire;
}

PlugNumber WireBackend::getPlugIndex(PlugType plugType) const
{
    PlugNumber result = INVALID;

    switch (plugType)
    {
    case PlugType::DataIn:
        result = m_inPlugNumber;
        break;

    case PlugType::DataOut:
        result = m_outPlugNumber;

        break;

    default:
        break;
    }

    return result;
}


void WireBackend::setCasingToPlug(CasingBackend* casingBackend, PlugType plugType, PlugNumber plugNumber)
{
    bool wasIncomplete = !complete();
    auto& weakCasingBackend = getCasing(plugType);
    weakCasingBackend = casingBackend;

    if (plugType == PlugType::DataOut)
        m_outPlugNumber = plugNumber;
    else
        m_inPlugNumber = plugNumber;

    m_state.setNoRequiredPlug();

    updated(*this);
    if (complete() && wasIncomplete)
    {
        wireBackendCompleted(*this);
    }
}


void WireBackend::removeFromCasings() const
{
    if (m_inCasingBackend)
        m_inCasingBackend->casingState().eraseWire(PlugType::DataIn, m_inPlugNumber, id());

    if (m_outCasingBackend)
        m_outCasingBackend->casingState().eraseWire(PlugType::DataOut, m_outPlugNumber, id());
}

Wire *WireBackend::getWire() const
{
    return m_wire;
}

WireState& WireBackend::state()
{
    return m_state;
}

WireState const& WireBackend::state() const
{
    return m_state;
}

CasingBackend* WireBackend::getCasing(PlugType plugType) const
{
    if(plugType == PlugType::DataIn)
        return m_inCasingBackend;
    else
        return m_outCasingBackend;
}

CasingBackend*& WireBackend::getCasing(PlugType plugType)
{
    if(plugType == PlugType::DataIn)
        return m_inCasingBackend;
    else
        return m_outCasingBackend;

}

void WireBackend::clearCasingBackend(PlugType plugType)
{
    if (complete())
    {
        wireBackendMadeIncomplete(*this);
    }

    getCasing(plugType) = nullptr;

    if (plugType == PlugType::DataIn)
        m_inPlugNumber = INVALID;
    else
        m_outPlugNumber = INVALID;
}

int WireBackend::dataType(PlugType plugType) const
{
    if (m_inCasingBackend && m_outCasingBackend)
    {
        auto const & idea = (plugType == PlugType::DataIn) ?
                    m_inCasingBackend->idea() :
                    m_outCasingBackend->idea();
        PlugNumber index = (plugType == PlugType::DataIn) ?
                    m_inPlugNumber :
                    m_outPlugNumber;

        int candidateCasingDataType;
        if(plugType == PlugType::DataIn)
            candidateCasingDataType = idea->attributes().dataTypesIn()[index];
        else
            candidateCasingDataType = idea->attributes().dataTypesOut()[index];

        return candidateCasingDataType;
    }
    else
    {
        CasingBackend* validCasingBackend;
        PlugNumber index = INVALID;

        if ((validCasingBackend = m_inCasingBackend))
        {
            index    = m_inPlugNumber;
            plugType = PlugType::DataIn;
        }
        else if ((validCasingBackend = m_outCasingBackend))
        {
            index    = m_outPlugNumber;
            plugType = PlugType::DataOut;
        }

        if (validCasingBackend)
        {
            auto const &idea = validCasingBackend->idea();

            int candidateCasingDataType;
            if(plugType == PlugType::DataIn)
                candidateCasingDataType = idea->attributes().dataTypesIn()[index];
            else
                candidateCasingDataType = idea->attributes().dataTypesOut()[index];

            return candidateCasingDataType;
        }
    }

    qFatal("Invalid datatype in WireBackend");
}

int WireBackend::dataType() const
{
    if(m_inCasingBackend)
    {
        auto const & idea = m_inCasingBackend->idea();

        return idea->attributes().dataTypesIn()[m_inPlugNumber];
    }
    if(m_outCasingBackend)
    {
        auto const & idea = m_outCasingBackend->idea();
        return idea->attributes().dataTypesOut()[m_outPlugNumber];
    }
    qFatal("WireBackend without datatype");
}

void WireBackend::propagateData(std::shared_ptr<DataType> dataType) const
{
    if(m_inCasingBackend)
        m_inCasingBackend->sendDataToIdea(dataType, m_inPlugNumber);
}

void WireBackend::propagateEmptyData() const
{
    std::shared_ptr<DataType> emptyData;

    propagateData(emptyData);
}
