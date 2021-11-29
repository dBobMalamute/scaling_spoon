#include "CorkboardBackend.h"

CorkboardBackend::CorkboardBackend(std::shared_ptr<IdeaRegistry> ideaRegistry, QObject * parent):
    QObject(parent), m_ideaRegistry(std::move(ideaRegistry))
{
    // This wireBackend should come first
    connect(this, &CorkboardBackend::wireCreated, this, &CorkboardBackend::setupWireSignals);
}

CorkboardBackend::~CorkboardBackend()
{
    clearData();
}

//------------------------------------------------------------------------------

std::shared_ptr<WireBackend> CorkboardBackend::createWireBackend(PlugType connectedPlugType, CasingBackend* casingBackend,
                                                                 PlugNumber plugNumber)
{
    auto wireBackend = std::make_shared<WireBackend>(connectedPlugType, casingBackend, plugNumber);
    m_wireBackends[wireBackend->id()] = wireBackend;

    // Note: this wireBackend isn't truly created yet. It's only partially created.
    connect(wireBackend.get(), &WireBackend::wireBackendCompleted,[this](WireBackend const& c)
    {
        wireCreated(c);
    });
    return wireBackend;
}

std::shared_ptr<WireBackend> CorkboardBackend::createWireBackend(QUuid id,
                                                               CasingBackend* inCasomgBackend, PlugNumber inPlugNumber,
                                                               CasingBackend* outCasingBackend, PlugNumber outPlugNumber)
{
    auto wireBackend = std::make_shared<WireBackend>(id, inCasomgBackend, inPlugNumber, outCasingBackend, outPlugNumber);

    m_wireBackends[wireBackend->id()] = wireBackend;
    wireCreated(*wireBackend);

    inCasomgBackend->casingState().setWire(PlugType::DataIn, inPlugNumber, *wireBackend);
    outCasingBackend->casingState().setWire(PlugType::DataOut, outPlugNumber, *wireBackend);

    return wireBackend;
}

std::shared_ptr<WireBackend> CorkboardBackend::restoreWireBackend(QJsonObject const &wireBackendJson)
{
    QUuid wireID = QUuid(wireBackendJson["id"].toString());

    QUuid casingInId  = QUuid(wireBackendJson["i_id"].toString());
    QUuid casingOutId = QUuid(wireBackendJson["o_id"].toString());

    PlugNumber plugNumberIn  = wireBackendJson["i_n"].toInt();
    PlugNumber plugNumberOut = wireBackendJson["o_n"].toInt();

    auto casingIn  = m_casingBackends[casingInId].get();
    auto casingOut = m_casingBackends[casingOutId].get();

    std::shared_ptr<WireBackend> wire = createWireBackend(wireID, casingIn, plugNumberIn, casingOut, plugNumberOut);

    return wire;
}

void CorkboardBackend::removeWire(WireBackend* wireBackend)
{
    auto it = m_wireBackends.find(wireBackend->id());
    if (it != m_wireBackends.end())
    {
        wireBackend->removeFromCasings();
        m_wireBackends.erase(it);
    }
}

void CorkboardBackend::removeWire(QUuid id)
{
    WireBackend* con = m_wireBackends[id].get();
    con->removeFromCasings();

    auto it = m_wireBackends.find(id);
    m_wireBackends.erase(it);
}

CasingBackend& CorkboardBackend::createCasingBackend(std::unique_ptr<Idea> && idea)
{
    auto casingBackend = std::make_unique<CasingBackend>(std::move(idea));

    auto casingBackendPtr = casingBackend.get();
    m_casingBackends[casingBackend->id()] = std::move(casingBackend);

    return *casingBackendPtr;
}

CasingBackend& CorkboardBackend::restoreCasingBackend(std::unique_ptr<Idea> && idea, Casing *casing, QJsonObject const& json)
{
    auto casingBackend = std::make_unique<CasingBackend>(std::move(idea));
    casingBackend->setCasing(casing);
    casingBackend->load(json);

    auto casingBackendPtr = casingBackend.get();
    m_casingBackends[casingBackend->id()] = std::move(casingBackend);
    return *casingBackendPtr;
}

QJsonArray CorkboardBackend::removeCasingBackend(CasingBackend *casingBackend)
{
    QJsonArray wiresJsonArray;
    for(auto plugType: {PlugType::DataIn,PlugType::DataOut})
    {
        auto state = casingBackend->casingState();
        auto const & activeWires = state.getEntries(plugType);

        for (auto &wires : activeWires)
        {
            for (auto const &pair : wires)
            {
                wiresJsonArray.append(pair.second->serialize());
                removeWire(pair.second);
            }
        }
    }
    m_casingBackends.erase(casingBackend->id());

    return wiresJsonArray;
}

void CorkboardBackend::removeCasingBackend(CasingBackend &casingBackend)
{
    QJsonArray wiresJsonArray;

    for(auto plugType: {PlugType::DataIn,PlugType::DataOut})
    {
        auto state = casingBackend.casingState();
        auto const & activeWires = state.getEntries(plugType);

        for (auto &wires : activeWires)
        {
            for (auto const &pair : wires)
            {
                wiresJsonArray.append(pair.second->serialize());
                removeWire(pair.second);
            }
        }
    }
    m_casingBackends.erase(casingBackend.id());
}

QJsonArray CorkboardBackend::removeCasingBackend(QUuid id)
{
    CasingBackend* casingBackend = m_casingBackends[id].get();

    QJsonArray wiressJsonArray;

    for(auto plugType: {PlugType::DataIn,PlugType::DataOut})
    {
        auto state = casingBackend->casingState();
        auto const & activeWires = state.getEntries(plugType);

        for (auto &wires : activeWires)
        {
            for (auto const &pair : wires)
            {
                wiressJsonArray.append(pair.second->serialize());
                removeWire(pair.second);
            }
        }
    }
    m_casingBackends.erase(casingBackend->id());
    return wiressJsonArray;
}

IdeaRegistry& CorkboardBackend::registry() const
{
    return *m_ideaRegistry;
}

void CorkboardBackend::setRegistry(std::shared_ptr<IdeaRegistry> registry)
{
    m_ideaRegistry = std::move(registry);
}

std::unordered_map<QUuid, std::unique_ptr<CasingBackend>> const & CorkboardBackend::casings() const
{
    return m_casingBackends;
}

std::unordered_map<QUuid, std::shared_ptr<WireBackend> > const & CorkboardBackend::wires() const
{
    return m_wireBackends;
}

//------------------------------------------------------------------------------

void CorkboardBackend::clearData()
{
    while (m_wireBackends.size() > 0)
    {
        removeWire( m_wireBackends.begin()->second.get() );
    }

    while (m_casingBackends.size() > 0)
    {
        removeCasingBackend( *m_casingBackends.begin()->second );
    }
}

QJsonObject CorkboardBackend::saveToMemory() const
{
    QJsonObject sceneJson;
    QJsonArray casingsJsonArray;

    for (auto const & pair : m_casingBackends)
    {
        auto const &casingBackend = pair.second;

        casingsJsonArray.append(casingBackend->save());
    }

    sceneJson["casings"] = casingsJsonArray;

    QJsonArray wireJsonArray;
    for (auto const & pair : m_wireBackends)
    {
        auto const &wire = pair.second;

        QJsonObject wireJson = wire->serialize();

        if (!wireJson.isEmpty())
            wireJsonArray.append(wireJson);
    }

    sceneJson["wires"] = wireJsonArray;
    return sceneJson;
}

QJsonObject CorkboardBackend::saveSelection() const
{
    QJsonObject selectionJson;
    QJsonArray casingsJsonArray;

    for (auto const & pair : m_casingBackends)
    {
        auto const &casingBackend = pair.second;

        if(casingBackend->casing().selected())
            casingsJsonArray.append(casingBackend->save());
    }

    selectionJson["casings"] = casingsJsonArray;

    QJsonArray wiresJsonArray;
    for (auto const & pair : m_wireBackends)
    {
        auto const &wireBackend = pair.second;
        if(wireBackend->getWire()->selected())
        {
            QJsonObject wireJson = wireBackend->serialize();

            if (!wireJson.isEmpty())
                wiresJsonArray.append(wireJson);
        }
    }

    selectionJson["wires"] = wiresJsonArray;

    return selectionJson;
}

ulong CorkboardBackend::numberWires()
{
    return m_wireBackends.size();
}

ulong CorkboardBackend::numberCasings()
{
    return m_casingBackends.size();
}

CasingBackend *CorkboardBackend::getCasing(QUuid id)
{
    return m_casingBackends[id].get();
}

WireBackend *CorkboardBackend::getWireBackends(QUuid id)
{
    return m_wireBackends[id].get();
}

void CorkboardBackend::setupWireSignals(WireBackend const& c)
{
    connect(&c, &WireBackend::wireBackendMadeIncomplete, this,
            &CorkboardBackend::wireDeleted, Qt::UniqueConnection);
}
