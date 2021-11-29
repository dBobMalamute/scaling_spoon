#ifndef CORKBOARDBACKEND_H
#define CORKBOARDBACKEND_H

#include <unordered_map>
#include <tuple>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "Registries/IdeaRegistry.h"
#include "CasingBackend.h"
#include "Corkboard.h"

//Backend to the corkboard.
//This class holds all the backend object: wireBackends and casingBackends.
//It saves and loads them
class CorkboardBackend: public QObject
{
    Q_OBJECT

public:
    CorkboardBackend(std::shared_ptr<IdeaRegistry> ideaRegistry, QObject * parent = nullptr);
    ~CorkboardBackend();

public:
    std::shared_ptr<WireBackend> createWireBackend(PlugType connectedPlugType,
                                                   CasingBackend *casingBackend, PlugNumber plugNumber);
    std::shared_ptr<WireBackend> createWireBackend(QUuid id, CasingBackend* inCasingBackend, PlugNumber inPlugNumber,
                                                   CasingBackend* outCasingBackend, PlugNumber outPlugNumber);
    std::shared_ptr<WireBackend> restoreWireBackend(QJsonObject const &wireJson);

    void removeWire(WireBackend *wireBackend);
    void removeWire(QUuid id);

    CasingBackend& createCasingBackend(std::unique_ptr<Idea> && idea);

    CasingBackend& restoreCasingBackend(std::unique_ptr<Idea> && idea, Casing* casing, QJsonObject const & json);

    QJsonArray removeCasingBackend(CasingBackend* casingBackend);
    void removeCasingBackend(CasingBackend& casingBackend);
    QJsonArray removeCasingBackend(QUuid id);

    IdeaRegistry&registry() const;
    void setRegistry(std::shared_ptr<IdeaRegistry> registry);

    CasingBackend* getCasing(QUuid id);
    WireBackend* getWireBackends(QUuid id);

public:
    std::unordered_map<QUuid, std::unique_ptr<CasingBackend>> const & casings() const;
    std::unordered_map<QUuid, std::shared_ptr<WireBackend> > const & wires() const;

public:
    void clearData();
    QJsonObject saveToMemory() const;
    QJsonObject saveSelection() const;
    ulong numberWires();
    ulong numberCasings();

signals:
    void wireCreated(WireBackend const &c);
    void wireDeleted(WireBackend const &c);

private:
    using SharedWireBackend = std::shared_ptr<WireBackend>;
    using UniqueCasingBackend = std::unique_ptr<CasingBackend>;

    std::unordered_map<QUuid, SharedWireBackend> m_wireBackends;
    std::unordered_map<QUuid, UniqueCasingBackend> m_casingBackends;
    std::shared_ptr<IdeaRegistry> m_ideaRegistry;

private slots:
    void setupWireSignals(WireBackend const& c);
};

#endif

