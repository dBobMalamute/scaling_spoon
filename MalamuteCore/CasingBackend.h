#ifndef CASINGBACKEND_H
#define CASINGBACKEND_H

#include <memory>

#include <QObject>
#include <QUuid>
#include <QJsonObject>
#include <QQmlContext>
#include <QQuickItem>

#include "Plug.h"
#include "CasingState.h"
#include "InheritedClasses/DataType.h"

class WireBackend;
class Casing;
class Idea;

class CasingBackend: public QObject
{
    Q_OBJECT

public:
    CasingBackend(std::unique_ptr<Idea> && idea);
    ~CasingBackend() override;

    QJsonObject save() const;
    void load(QJsonObject const &json);

    void finishSetup(QJsonObject loadJson);

    QUuid id() const;
    void moveWires() const;

    Casing const &casing() const;
    Casing & casing();

    void setCasing(Casing* casing);

    CasingState const & casingState() const;
    CasingState & casingState();

    Idea* idea() const;

    QQmlContext *getContext() const;
    void prepareContext(QQmlContext *value);

    void loadEmbeddedQML();
    void deleteEmbeddedQML();
    QQuickItem *getEmbeddedQML() const;

    void loadMinimizedQML();
    void deleteMinimizedQML();
    QQuickItem *getMinimizedQML() const;

    void loadExternalQML();
    QList<QQuickItem*> getExternalQML() const;
    /// Deletes an external QML FILE. default parameter removes all items.
    void onDeleteExternalQML(int i = -1);

    void setEmbeddedQMLEnabled(bool enabled);

    void addGear();

    void addStopLight(StopLight stopLight);
    void addRedLight(PlugType plugType, PlugNumber plugNumber);
    void addGreenLight(PlugType plugType, PlugNumber plugNumber);
    void clearStopLights();

    bool getEmbeddedQMLLoaded() const;
    bool getMinimizedQMLLoaded() const;
    bool getExternalQMLLoaded() const;

    void createPostIt();

public slots: // data propagation

    /// Propagates incoming data to the underlying model.
    void sendDataToIdea(std::shared_ptr<DataType> dataType, PlugNumber inPlugIndex) const;

    /// Fetches data from model's OUT #index plug
    void onNewData(PlugNumber plugNumber);
    void onNewDataAllPlugs();
    void onClearDataAllPlugs();
    void sendDataNewWire(QUuid id);

    /// Updates the information for the visual casing.
    void onStatusChanged();
    void onStartSpinningGear();
    void onStopSpinningGear();

    /// Registers a new Context Property.
    /// This allows for individual ideas to have there own context properties.
    /// Useful for model/view programming.
    void onSetContextProperty(QString name, QObject* obj);

    /// Reports the change in paramaters in one file to the undostack.
    void onReportParamsChanged(QJsonObject oldParams, QJsonObject newParams);

private:
    QUuid m_uid;
    std::unique_ptr<Idea> m_idea;
    CasingState m_casingState;

    QVector<QQuickItem*> m_stopLights;
    QVector<bool> m_inPlugHasStopLight;
    QVector<bool> m_outPlugHasStopLight;

    // Each casing backend holds a qmlContext. This allows the casing to pass data between
    // the idea and the custom qml component.
    QQmlContext* m_context;

    QQuickItem* m_embeddedQML;
    double m_embeddedWidthFromLoad;
    double m_embeddedHeightFromLoad;
    bool m_embeddedQMLLoaded;

    QQuickItem* m_minimizedQML;
    bool m_minimizedQMLLoaded;

    QList<QQuickItem*> m_externalQML;
    bool m_externalQMLLoaded;

    // The front-end part of this abstraction.
    Casing* m_casing;

    QQuickItem* m_gear;

};

#endif // CASINGBACKEND_H
