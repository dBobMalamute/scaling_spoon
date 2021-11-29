#include "CasingBackend.h"

#include <utility>
#include <iostream>

#include "Casing.h"
#include "InheritedClasses/Idea.h"
#include "Corkboard.h"

CasingBackend::CasingBackend(std::unique_ptr<Idea> && idea) :
    m_uid(QUuid::createUuid()),
    m_idea(std::move(idea)),
    m_casingState(m_idea),
    m_embeddedWidthFromLoad(-1.0),
    m_embeddedHeightFromLoad(-1.0),
    m_embeddedQMLLoaded(false),
    m_minimizedQMLLoaded(false),
    m_externalQMLLoaded(false),
    m_casing(nullptr),
    m_gear(nullptr)
{
    m_externalQML.clear();
}

CasingBackend::~CasingBackend()
{
    if(m_gear)
        delete m_gear;

    deleteEmbeddedQML();
    deleteMinimizedQML();

    for(int i = 0; i < m_externalQML.size(); i++)
    {
        delete m_externalQML[i];
    }

    delete m_casing;
}

QJsonObject CasingBackend::save() const
{
    QJsonObject casingJson;

    casingJson["id"] = m_uid.toString();
    casingJson["name"] = m_idea->attributes().listName();
    casingJson["idea"] = m_idea->save();
    casingJson["comp"] = m_casing->save();
    if(m_embeddedQMLLoaded && m_casing->resizeable())
    {
        casingJson["ew"] = m_embeddedQML->width();
        casingJson["eh"] = m_embeddedQML->height();
    }

    return casingJson;
}

void CasingBackend::load(QJsonObject const& json)
{
    m_uid = QUuid(json["id"].toString());
    m_casing->load(json["comp"].toObject());

    QJsonValue ew = json["ew"];
    if(!ew.isUndefined())
        m_embeddedWidthFromLoad = ew.toDouble();
    QJsonValue eh = json["eh"];
    if(!eh.isUndefined())
        m_embeddedHeightFromLoad = eh.toDouble();
}

void CasingBackend::finishSetup(QJsonObject loadJson)
{
    //This class handles all the signals from the idea class.
    //The idea class shouldn't know or depend on any details of the casing class.

    //InterIdea Communication Handlers
    connect(m_idea.get(), &Idea::newData, this, &CasingBackend::onNewData);
    connect(m_idea.get(), &Idea::newDataAllPlugs, this, &CasingBackend::onNewDataAllPlugs);
    connect(m_idea.get(), &Idea::clearDataAllPlugs, this, &CasingBackend::onClearDataAllPlugs);

    //Within Idea handlers
    connect(m_idea.get(), &Idea::statusChanged, this, &CasingBackend::onStatusChanged);
    connect(m_idea.get(), &Idea::reportParamsChanged,this,&CasingBackend::onReportParamsChanged);
    connect(m_idea.get(), &Idea::setContextProperty, this, &CasingBackend::onSetContextProperty);
    connect(m_idea.get(), &Idea::deleteExternalQML, this, &CasingBackend::onDeleteExternalQML);

    //Create and prepare a gear if there is one.
    if(m_idea->attributes().hasGear())
    {
        addGear();
        connect(m_idea.get(), &Idea::startSpinningGear, this, &CasingBackend::onStartSpinningGear);
        connect(m_idea.get(), &Idea::stopSpinningGear, this, &CasingBackend::onStopSpinningGear);
    }

    //Load the initial things from the idea. Finish setup of qml things if necessary.
    m_idea->finishSetupQmlContext();
    m_idea->load(loadJson);
}

QUuid CasingBackend::id() const
{
    return m_uid;
}

void CasingBackend::moveWires() const
{
    for (PlugType plugType: {PlugType::DataIn, PlugType::DataOut})
    {
        auto const & allWires = m_casingState.getEntries(plugType);

        for (auto const & wires : allWires)
        {
            for (auto & wire : wires)
            {
                wire.second->getWire()->move();
            }
        }
    }
}

Casing const & CasingBackend::casing() const
{
    return *m_casing;
}

Casing & CasingBackend::casing()
{
    return *m_casing;
}

void CasingBackend::setCasing(Casing *casing)
{
    m_casing = std::move(casing);
}

CasingState const & CasingBackend::casingState() const
{
    return m_casingState;
}

CasingState & CasingBackend::casingState()
{
    return m_casingState;
}

Idea* CasingBackend::idea() const
{
    return m_idea.get();
}

void CasingBackend::sendDataToIdea(std::shared_ptr<DataType> dataType, PlugNumber inPlugNumber) const
{
    m_idea->onNewDataIn(std::move(dataType), inPlugNumber);

    m_casing->recalculateSize();
    m_casing->update();
}

void CasingBackend::onNewData(PlugNumber index)
{
    IdeaStatus status = m_idea->status();

    // Boxes should not send data in standby error or fatal states.
    if(status == IdeaStatus::StandBy || status == IdeaStatus::Error || status == IdeaStatus::Fatal)
        return;

    auto ideaData = m_idea->dataOut(index);
    auto wires = m_casingState.wires(PlugType::DataOut, index);
    for (auto const & c : wires)
    {
        c.second->propagateData(ideaData);
        c.second->getWire()->setTransmitting(true);
    }
}

void CasingBackend::onNewDataAllPlugs()
{
    IdeaStatus status = m_idea->status();

    // Boxes should not send data in standby error or fatal states.
    if(status == IdeaStatus::StandBy || status == IdeaStatus::Error || status == IdeaStatus::Fatal)
        return;

    auto wiresVector = m_casingState.wires(PlugType::DataOut);
    for (ulong i = 0; i < wiresVector.size(); i++)
    {
        auto ideaData = m_idea->dataOut(static_cast<int>(i));
        for(auto const & c: wiresVector[i])
        {
            c.second->propagateData(ideaData);
            c.second->getWire()->setTransmitting(true);
        }
    }
}

void CasingBackend::onClearDataAllPlugs()
{
    IdeaStatus status = m_idea->status();

    // Never silently clear the output. That cost me way to much time figuring out a bug.
    if(status == IdeaStatus::Working || status == IdeaStatus::Warning || status == IdeaStatus::DisplayError)
    {
        qFatal("Clear data called on valid idea. This is super confusing for the user. Don't do it");
        return;
    }
    auto wiresVector = m_casingState.wires(PlugType::DataOut);

    for (ulong i = 0; i < wiresVector.size(); i++)
    {
        for(auto const & c: wiresVector[i])
        {
            c.second->propagateEmptyData();
        }
    }
}

void CasingBackend::sendDataNewWire(QUuid id)
{
    IdeaStatus status = m_idea->status();
    // Boxes only send data when they are working or warning.
    if(status == IdeaStatus::StandBy || status == IdeaStatus::Error || status == IdeaStatus::Fatal)
        return;

    auto wiresVector = m_casingState.wires(PlugType::DataOut);
    for (ulong i = 0; i < wiresVector.size(); i++)
    {
        for(auto const & c: wiresVector[i])
        {
            if(c.first == id)
            {
                auto ideaData = m_idea->dataOut(static_cast<int>(i));
                c.second->propagateData(ideaData);
                return;
            }
        }
    }
    qFatal("Send data called on invalid ID");
}

void CasingBackend::onStatusChanged()
{
    if(m_casing)
        m_casing->setupValidationBox();

    //The backend is created before the casing.
    //Sometimes the constructor or restore function of the idea sends instructions to the casing
    //before the casing is complete. The error checking here slows things down a tiny amount to ensure
    //that coding is easier.
}

void CasingBackend::onStartSpinningGear()
{
    m_gear->setProperty("rotating", true);
}

void CasingBackend::onStopSpinningGear()
{
    m_gear->setProperty("rotating", false);
}

void CasingBackend::onSetContextProperty(QString name, QObject *obj)
{
    m_context->setContextProperty(name, obj);
}

void CasingBackend::onReportParamsChanged(QJsonObject oldParams, QJsonObject newParams)
{
    m_casing->corkboard()->reportParamsChanged(id(),oldParams, newParams);
}

bool CasingBackend::getExternalQMLLoaded() const
{
    return m_externalQMLLoaded;
}

bool CasingBackend::getMinimizedQMLLoaded() const
{
    return m_minimizedQMLLoaded;
}

bool CasingBackend::getEmbeddedQMLLoaded() const
{
    return m_embeddedQMLLoaded;
}

QQmlContext *CasingBackend::getContext() const
{
    return m_context;
}

void CasingBackend::prepareContext(QQmlContext *value)
{
    m_context = value;
    m_context->setContextProperty("idea",m_idea.get());
}

QQuickItem *CasingBackend::getEmbeddedQML() const
{
    return m_embeddedQML;
}

void CasingBackend::loadEmbeddedQML()
{
    QString path = idea()->attributes().embeddedQMLPath();
    QQmlComponent qmlcomponent(m_casing->corkboard()->qmlEngine(),QUrl::fromLocalFile(path));

    QObject* preEmbeddedQML = qmlcomponent.beginCreate(m_context);
    if(!preEmbeddedQML)
        qDebug() << "Failed to create Object.";

    //To Make the qml component visible, it must have a parent.
    //First we cast it to be a QQuickItem. Then we set the parent to the be qml.
    // This ensures it stays with the qml.
    m_embeddedQML = qobject_cast<QQuickItem*>(preEmbeddedQML);
    if(m_embeddedQML)
    {
        m_embeddedQML->setParentItem(m_casing);

        if(m_embeddedHeightFromLoad > 0.0 && m_embeddedWidthFromLoad > 0.0)
        {
            m_embeddedQML->setWidth(m_embeddedWidthFromLoad);
            m_embeddedQML->setHeight(m_embeddedHeightFromLoad);
        }

        // This ensures that resizing the embedded qml works.
        connect(m_embeddedQML, &QQuickItem::widthChanged, m_casing, &Casing::recalculateSize);
        connect(m_embeddedQML, &QQuickItem::heightChanged, m_casing, &Casing::recalculateSize);
        m_embeddedQMLLoaded = true;
        qmlcomponent.completeCreate();
    }
}

void CasingBackend::deleteEmbeddedQML()
{
    if(m_embeddedQMLLoaded)
    {
        m_embeddedQMLLoaded = false;
        disconnect(m_embeddedQML, &QQuickItem::widthChanged, m_casing, &Casing::recalculateSize);
        disconnect(m_embeddedQML, &QQuickItem::heightChanged, m_casing, &Casing::recalculateSize);
        delete m_embeddedQML;
    }
}

QQuickItem *CasingBackend::getMinimizedQML() const
{
    return m_minimizedQML;
}

void CasingBackend::loadMinimizedQML()
{
    QString path = idea()->attributes().minimizedQMLPath();
    QQmlComponent qmlcomponent(m_casing->corkboard()->qmlEngine(),QUrl::fromLocalFile(path));

    QObject* preMinimizedQML = qmlcomponent.beginCreate(m_context);
    if(!preMinimizedQML)
        qDebug() << "Failed to create Object.";

    //To Make the qml component visible, it must have a parent.
    //First we cast it to be a QQuickItem. Then we set the parent to the be qml.
    // This ensures it stays with the qml.
    m_minimizedQML = qobject_cast<QQuickItem*>(preMinimizedQML);
    if(m_minimizedQML)
    {
        m_minimizedQML->setParentItem(m_casing);

        m_minimizedQMLLoaded = true;
        qmlcomponent.completeCreate();
    }
}

void CasingBackend::deleteMinimizedQML()
{
    if(m_minimizedQMLLoaded)
    {
        m_minimizedQMLLoaded = false;
        delete m_minimizedQML;
    }
}

void CasingBackend::addStopLight(StopLight stopLight)
{
    QUrl url(QStringLiteral("qrc:/QML/StopLight.qml"));
    QQmlComponent component(m_casing->corkboard()->qmlEngine(),url);

    QObject* preStopLight = component.create(m_context);
    if(!preStopLight)
        qDebug() << "failed to create object";

    //Find the y coordinate.

    QQuickItem* qmlStopLight = qobject_cast<QQuickItem*>(preStopLight);
    if(qmlStopLight)
    {
        qmlStopLight->setProperty("stopLightColor",stopLight.m_color);
        QPointF position = m_casing->plugScenePosition(stopLight.m_plugNumber, stopLight.m_plugType);
        qmlStopLight->setPosition(position - QPointF(11,11));
        qmlStopLight->setParentItem(m_casing);
    }
    m_stopLights.append(qmlStopLight);
}

void CasingBackend::addRedLight(PlugType plugType, PlugNumber plugNumber)
{
    StopLight stopLight(plugType,plugNumber,QColor("#b31212"));
    if(plugType == PlugType::DataIn)
    {
        if(!m_inPlugHasStopLight[plugNumber])
        {
            m_inPlugHasStopLight[plugNumber] = true;
            addStopLight(stopLight);
        }
        return;
    }
    if(plugType == PlugType::DataOut)
    {
        if(!m_outPlugHasStopLight[plugNumber])
        {
            m_outPlugHasStopLight[plugNumber] = true;
            addStopLight(stopLight);
        }
        return;
    }
    return;
}

void CasingBackend::addGreenLight(PlugType plugType, PlugNumber plugNumber)
{
    StopLight stopLight(plugType,plugNumber,QColor("#12b33c"));

    if(plugType == PlugType::DataIn)
    {
        if(!m_inPlugHasStopLight[plugNumber])
        {
            m_inPlugHasStopLight[plugNumber] = true;
            addStopLight(stopLight);
        }
        return;
    }
    if(plugType == PlugType::DataOut)
    {
        if(!m_outPlugHasStopLight[plugNumber])
        {
            m_outPlugHasStopLight[plugNumber] = true;
            addStopLight(stopLight);
        }
        return;
    }
    return;
}

void CasingBackend::clearStopLights()
{
    foreach (QQuickItem* item, m_stopLights)
    {
        delete item;
    }

    m_inPlugHasStopLight.clear();
    m_outPlugHasStopLight.clear();

    for(int i = 0; i < m_casing->inPlugLabels().length(); i++)
        m_inPlugHasStopLight.append(false);
    for(int i = 0; i < m_casing->outPlugLabels().length(); i++)
        m_outPlugHasStopLight.append(false);

    m_stopLights.clear();
}

QList<QQuickItem *> CasingBackend::getExternalQML() const
{
    return m_externalQML;
}

void CasingBackend::createPostIt()
{
    IdeaAttributes attributes = m_idea.get()->attributes();

    double x = casing().x() + attributes.postitLocation().extraXOffset();
    if(attributes.postitLocation().offsetByWidth())
        x += casing().width();

    double y = casing().y() + attributes.postitLocation().extraYOffset();
    if(attributes.postitLocation().offsetByHeight())
        y+= casing().height();

    QString title = attributes.listName();
    QString shortMessage = m_idea->shortStatusMessage();
    QString longMessage = attributes.description() + "\n\n" + m_idea->longStatusMessage();
    m_casing->corkboard()->createPostIt(QPointF(x,y), title, shortMessage,longMessage);
}

void CasingBackend::loadExternalQML()
{
    IdeaAttributes attributes = m_idea.get()->attributes();

    if(m_externalQML.size() > 0)
    {
        //The External QML has already been loaded.
        return;
    }
    else
    {
        m_externalQMLLoaded = true;
        for(int i = 0; i < attributes.externalQMLPaths().size(); i++)
        {
            QString path = attributes.externalQMLPaths()[i];
            double x = attributes.externalQMLLocations()[i].extraXOffset();
            if(attributes.externalQMLLocations()[i].offsetByWidth())
                x += casing().width();

            double y = attributes.externalQMLLocations()[i].extraYOffset();
            if(attributes.externalQMLLocations()[i].offsetByHeight())
                y+= casing().height();

            QQmlComponent qmlComponent(m_casing->corkboard()->qmlEngine(),QUrl::fromLocalFile(path));
            QObject* preExternalQML = qmlComponent.create(m_context);
            if(!preExternalQML)
                qDebug() << "Failed to create Object";
            m_externalQML.append(qobject_cast<QQuickItem*>(preExternalQML));
            m_externalQML[i]->setParentItem(m_casing);
            m_externalQML[i]->setPosition(QPointF(x,y));
        }
    }
}

void CasingBackend::onDeleteExternalQML(int i)
{
    if(i == -1)
    {
        for(int j = 0; j < m_externalQML.size(); j++)
            m_externalQML[j]->deleteLater();
        m_externalQML.clear();
        m_externalQMLLoaded = false;
        return;
    }
    m_externalQML[i]->deleteLater();
    m_externalQML.removeAt(i);
}

void CasingBackend::setEmbeddedQMLEnabled(bool enabled)
{
    if(m_embeddedQMLLoaded)
    {
        m_embeddedQML->setEnabled(enabled);
    }
}

void CasingBackend::addGear()
{
    QUrl url(QStringLiteral("qrc:/QML/CasingGear.qml"));
    QQmlComponent qqmlcomponent(m_casing->corkboard()->qmlEngine(),url);

    QObject* preGear = qqmlcomponent.beginCreate(m_context);
    if(!preGear)
        qFatal("Failed to create qml object");

    m_gear = qobject_cast<QQuickItem*>(preGear);
    if(m_gear)
    {
        m_gear->setParentItem(m_casing);
        qqmlcomponent.completeCreate();
    }
}
