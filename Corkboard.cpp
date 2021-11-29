#include "Corkboard.h"

#include "CorkboardBackend.h"

#include <QQmlApplicationEngine>

Corkboard::Corkboard(QQuickItem *parent) : QQuickItem(parent),
    m_undoStack(new QUndoStack(this)),
    m_selectionOrPanStartPoint(QPointF()),
    m_selecting(false),
    m_panning(false),
    m_drawGrid(true),
    m_contextMenuActive(false),
    m_visibleWindowWidth(1000),
    m_visibleWindowHeight(800),
    m_contextMenuPt(QPointF()),
    m_movingItems(),
    m_initialPositions()
{
    setFocus(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    setAcceptHoverEvents(true);
    setCursor(QCursor(QPixmap(":/Images/SmallerCursor.png"),2,2));
    zoomFactor = 1.1;
}

CorkboardBackend *Corkboard::backend() const
{
    return m_backend;
}

void Corkboard::createBackend(std::shared_ptr<IdeaRegistry> ideaRegistry)
{
    m_backend = new CorkboardBackend(ideaRegistry,this);
}

void Corkboard::mousePressEvent(QMouseEvent *event)
{   
    if(mapToScene(event->pos()).y() < 40)
    {
        // I don't want people to access the corkboard by clicking on unused areas of the header.
        // This prevents that.
        return;
    }

    if(event->buttons() == Qt::RightButton)
    {
        //Show the Context Menu
        setContextMenuPt(mapToScene(event->pos()));
        setContextMenuActive(true);

        setFocus(true);
        event->accept();
    }
    else if (event->buttons() == Qt::LeftButton && (event->modifiers() & Qt::ShiftModifier))
    {
        //Starting a selection box
        m_selectionOrPanStartPoint = event->pos();
        setSelecting(true);
        event->accept();
    }
    else if (event->buttons() == Qt::LeftButton)
    {
        // Click to clear selection, maybe start drag.
        deselectAll();
        setFocus(true);
        setContextMenuActive(false);
        m_selectionOrPanStartPoint = event->globalPos();
        m_startingPos = position();
        m_panning = true;
        event->accept();
    }
}

void Corkboard::mouseMoveEvent(QMouseEvent *event)
{
    if(m_panning)
    {
        QPointF panningCurrentPoint = event->globalPos();
        QPointF displacement = m_selectionOrPanStartPoint - panningCurrentPoint;
        setPosition(m_startingPos - displacement);
        event->accept();
    }
    else if(m_selecting)
    {
        QPointF selectionCurrentPoint = event->pos();
        setSelectingBox(QRectF(m_selectionOrPanStartPoint,selectionCurrentPoint).normalized());
        event->accept();
    }
}

void Corkboard::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_panning)
    {
        m_panning = false;
        QPointF panningCurrentPoint = event->globalPos();
        QPointF displacement = m_selectionOrPanStartPoint - panningCurrentPoint;
        setPosition(m_startingPos - displacement);
        event->accept();
    }
    if(m_selecting == true)
    {
        setSelecting(false);
        selectInBox(m_selectingBox);
        event->accept();
    }
}

QQmlContext *Corkboard::rootContext() const
{
    return m_rootContext;
}

QQmlEngine *Corkboard::qmlEngine() const
{
    return m_qmlEngine;
}

void Corkboard::setQmlEngine(QQmlEngine *qmlEngine)
{
    m_qmlEngine = qmlEngine;
    m_rootContext = m_qmlEngine->rootContext();
}

void Corkboard::undo()
{
    m_undoStack->undo();
}

void Corkboard::redo()
{
    m_undoStack->redo();
}

void Corkboard::cut()
{
    addInternalWiresToSelection();
    QJsonObject obj = m_backend->saveSelection();
    obj["postIts"] = saveSelectedPostIts();
    QByteArray byteArray = QJsonDocument(obj).toJson(QJsonDocument::Compact);

    QMimeData* mimeData = new QMimeData();
    mimeData->setData(corkboardOperationsMimeType,byteArray);
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);

    DeleteSelectionCommand* command = new DeleteSelectionCommand(obj,this);
    m_undoStack->push(command);
}

void Corkboard::copy()
{
    addInternalWiresToSelection();
    QJsonObject obj = m_backend->saveSelection();
    obj["postIts"] = saveSelectedPostIts();
    QByteArray byteArray = QJsonDocument(obj).toJson(QJsonDocument::Compact);

    QMimeData* mimeData = new QMimeData();
    mimeData->setData(corkboardOperationsMimeType,byteArray);
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

void Corkboard::paste()
{
    deselectAll();
    QPointF corkboardPos = mapFromGlobal(QCursor::pos());

    QClipboard * clipboard = QApplication::clipboard();
    const QMimeData* mimeData = clipboard->mimeData();

    if(!mimeData)
    {
        qDebug() << "Invalid data";
        return;
    }

    if(mimeData->hasFormat(corkboardOperationsMimeType))
    {
        QByteArray byteArray = mimeData->data(corkboardOperationsMimeType);
        QJsonObject const obj = QJsonDocument::fromJson(byteArray).object();
        AddSelectionCommand* command = new AddSelectionCommand(obj,this, corkboardPos, true);
        m_undoStack->push(command);
    }
}

void Corkboard::duplicate()
{
    addInternalWiresToSelection();
    QJsonObject obj = m_backend->saveSelection();
    obj["postIts"] = saveSelectedPostIts();

    deselectAll();

    if(!obj.empty())
    {
        QPointF corkboardPos = mapFromGlobal(QCursor::pos());
        AddSelectionCommand* command = new AddSelectionCommand(obj, this, corkboardPos, true);
        m_undoStack->push(command);
    }
}

void Corkboard::deleteSelection()
{
    QJsonObject obj = m_backend->saveSelection();
    obj["postIts"] = saveSelectedPostIts();
    DeleteSelectionCommand* command = new DeleteSelectionCommand(obj,this);
    m_undoStack->push(command);
}

void Corkboard::deleteAll()
{
    selectAll();
    deleteSelection();
}

void Corkboard::setVisibleWindowParams(int width, int height)
{
    m_visibleWindowWidth = width;
    m_visibleWindowHeight = height;
}

void Corkboard::centerView()
{
    setX(-1 * (width() - m_visibleWindowWidth) / 2);
    setY(-1 * (height() - m_visibleWindowHeight) / 2);
}

void Corkboard::resetScale()
{
    setScale(1.0);
}

void Corkboard::zoomIn()
{
    setScale(scale() * zoomFactor);
}

void Corkboard::zoomOut()
{
    setScale(scale() / zoomFactor);
}

void Corkboard::showDetailsFromCasingMenu()
{
    casingWhichCreatedMenu->casingBackend()->createPostIt();
}

void Corkboard::minimizeFromCasingMenu()
{
    QUuid id = casingWhichCreatedMenu->casingBackend()->id();
    bool oldState = casingWhichCreatedMenu->minimized();

    MinimizeCasingCommand* command = new MinimizeCasingCommand(id, this, oldState, true);
    m_undoStack->push(command);
}

void Corkboard::showControlsFromCasingMenu()
{
    casingWhichCreatedMenu->extraButtonClicked();
}

void Corkboard::setColorFromCasingMenu(int color)
{
    casingWhichCreatedMenu->setSelected(true);
    std::vector<Casing*> selectedCasings = this->selectedCasings();

    QList<QUuid> ids;
    QList<int> oldColors;

    for(size_t i = 0; i < selectedCasings.size(); i++)
    {
        int oldColor = selectedCasings.at(i)->predefinedColor();
        if(color != oldColor)
        {
            ids.append(selectedCasings.at(i)->casingBackend()->id());
            oldColors.append(oldColor);
        }
    }

    ChangeColorsCommand* command = new ChangeColorsCommand(ids, oldColors, color, this);
    m_undoStack->push(command);
}

void Corkboard::deleteFromCasingMenu()
{
    DeleteCasingCommand* command = new DeleteCasingCommand(
                casingWhichCreatedMenu->casingBackend()->save(), this);
    m_undoStack->push(command);
}

void Corkboard::copyStateFromCasingMenu()
{
    QJsonObject obj = casingWhichCreatedMenu->casingBackend()->idea()->save();

    QByteArray byteArray = QJsonDocument(obj).toJson(QJsonDocument::Compact);

    QMimeData* mimeData = new QMimeData();
    mimeData->setData(casingStateMimeType, byteArray);
    QClipboard* clipBoard = QApplication::clipboard();
    clipBoard->setMimeData(mimeData);
}

void Corkboard::pasteStateFromCasingMenu()
{
    QClipboard* clipBoard = QApplication::clipboard();
    const QMimeData* mimeData = clipBoard->mimeData();

    if(!mimeData)
    {
        return;
    }

    if(mimeData->hasFormat(casingStateMimeType))
    {
        QByteArray byteArray = mimeData->data(casingStateMimeType);
        QJsonObject const obj = QJsonDocument::fromJson(byteArray).object();

        QUuid id = casingWhichCreatedMenu->casingBackend()->id();
        QJsonObject oldParams = casingWhichCreatedMenu->casingBackend()->idea()->save();
        ChangeParamsCommand* command = new ChangeParamsCommand(id, oldParams, obj, this, true);
        m_undoStack->push(command);
    }
}

void Corkboard::zoomIn(QPointF pos)
{
    QPointF windowPos = mapToScene(pos);

    setScale(scale() * zoomFactor);
    double p_x = (pos.x() - width()/2.0) / width();
    double p_y = (pos.y() - height()/2.0) / height();

    setX(-width()/2.0 - p_x * width() * scale() + windowPos.x());
    setY(-height()/2.0 - p_y * height() * scale() + windowPos.y() - HEADER_HEIGHT);
}

void Corkboard::zoomOut(QPointF pos)
{
    QPointF windowPos = mapToScene(pos);

    setScale(scale() / zoomFactor);
    double p_x = (pos.x() - width()/2.0) / width();
    double p_y = (pos.y() - height()/2.0) / height();

    setX(-width()/2.0 - p_x * width() * scale() + windowPos.x());
    setY(-height()/2.0 - p_y * height() * scale() + windowPos.y() - HEADER_HEIGHT);
}

bool Corkboard::selecting() const
{
    return m_selecting;
}

bool Corkboard::contextMenuActive() const
{
    return m_contextMenuActive;
}

QPointF Corkboard::contextMenuPt() const
{
    return m_contextMenuPt;
}

void Corkboard::setSelecting(bool selecting)
{
    if (m_selecting == selecting)
        return;

    m_selecting = selecting;
    emit selectingChanged(m_selecting);
}
void Corkboard::setContextMenuActive(bool contextMenuActive)
{
    if (m_contextMenuActive == contextMenuActive)
        return;

    m_contextMenuActive = contextMenuActive;
    emit contextMenuActiveChanged(m_contextMenuActive);
}

void Corkboard::setContextMenuPt(QPointF contextMenuPt)
{
    if (m_contextMenuPt == contextMenuPt)
        return;

    m_contextMenuPt = contextMenuPt;
    emit contextMenuPtChanged(m_contextMenuPt);
}

void Corkboard::setCasingMenuActive(bool casingMenuActive)
{
    if (m_casingMenuActive == casingMenuActive)
        return;

    m_casingMenuActive = casingMenuActive;
    emit casingMenuActiveChanged(m_casingMenuActive);
}

void Corkboard::setCasingMenuPt(QPointF casingMenuPt)
{
    if (m_casingMenuPt == casingMenuPt)
        return;

    m_casingMenuPt = casingMenuPt;
    emit casingMenuPtChanged(m_casingMenuPt);
}

void Corkboard::addInternalWiresToSelection()
{
    std::vector<Casing*> casings = selectedCasings();
    QVector<QUuid> inIds;
    QVector<QUuid> outIds;

    //Loop over selected casings. Add all wire in plugs to them.
    for(ulong i = 0; i < casings.size(); i++)
    {
        inIds.append(casings[i]->casingBackend()->casingState().wireIds(PlugType::DataIn));
    }

    //Loop over selected casings. Add all wire out plugs to them.
    for(ulong i = 0; i < casings.size(); i++)
    {
        outIds.append(casings[i]->casingBackend()->casingState().wireIds(PlugType::DataOut));
    }

    //Compare loops set selected to true on duplicates.
    for(int i = 0; i < inIds.size(); i++)
    {
        for(int j = 0; j < outIds.size(); j++)
        {
            if(inIds[i] == outIds[j])
            {
                m_backend->getWireBackends(inIds[i])->getWire()->setSelected(true);
            }
        }
    }
}

Casing *Corkboard::createCasingBypassUndoStack(const QJsonObject &jsonObj)
{
    QString ideaName = jsonObj["name"].toString();
    std::unique_ptr<Idea> idea = m_backend->registry().createIdea(ideaName);

    QQmlComponent qmlComponent(qmlEngine(),QUrl(QStringLiteral("qrc:/QML/Casing.qml")));
    QObject* preCasing = qmlComponent.create(m_rootContext);
    Casing* casing = qobject_cast<Casing*>(preCasing);

    auto& casingBackend = m_backend->restoreCasingBackend(std::move(idea), casing, jsonObj);
    casingBackend.prepareContext(new QQmlContext(m_rootContext,&casingBackend));

    casing->setBackend(&casingBackend);
    casing->setCorkboard(this);
    casingBackend.finishSetup(jsonObj["idea"].toObject());
    casing->finishSetup();

    setFocus(true);

    return casing;
}

void Corkboard::setSelectingBox(QRectF selectingBox)
{
    if (m_selectingBox == selectingBox)
        return;

    m_selectingBox = selectingBox;
    emit selectingBoxChanged(m_selectingBox);
}

std::shared_ptr<DataTypeRegistry> Corkboard::dataTypeRegistry() const
{
    return m_dataTypeRegistry;
}

void Corkboard::setDataTypeRegistry(const std::shared_ptr<DataTypeRegistry> &dataTypeRegistry)
{
    m_dataTypeRegistry = dataTypeRegistry;
}

bool Corkboard::casingMenuActive() const
{
    return m_casingMenuActive;
}

QPointF Corkboard::casingMenuPt() const
{
    return m_casingMenuPt;
}

void Corkboard::readyCasingMenu(Casing *casing, QPointF point)
{
    casingWhichCreatedMenu = casing;
    setCasingMenuPt(point);
    setCasingMenuActive(true);
}

QRectF Corkboard::selectingBox() const
{
    return m_selectingBox;
}

void Corkboard::setWirePlugSwitch(const QJsonObject &wirePlugSwitch)
{
    m_wirePlugSwitch = wirePlugSwitch;
}

void Corkboard::createIdea(const QJsonObject &jsonObj)
{
    AddCasingCommand* command = new AddCasingCommand(jsonObj, this);
    m_undoStack->push(command);
}

void Corkboard::createIdea(QString name, QPointF point)
{
    deselectAll();
    QPointF corkboardPos =  mapFromScene(point);

    QJsonObject ideaJson;

    QJsonObject obj;
    obj["x"] = corkboardPos.x();
    obj["y"] = corkboardPos.y();

    QJsonObject casingJson;
    casingJson["idea"] = ideaJson;
    casingJson["id"] = QUuid::createUuid().toString();
    casingJson["name"] = name;
    casingJson["comp"] = obj;

    AddCasingCommand* command = new AddCasingCommand(casingJson, this);
    m_undoStack->push(command);
}

void Corkboard::createIdeaDefaultDisplay(QString name, QUuid sourceID, PlugNumber plug, QPointF eventPos)
{
    deselectAll();
    QPointF corkboardPos =  mapFromScene(eventPos + QPointF(100, 20));

    QJsonObject ideaJson;

    QJsonObject obj;
    obj["x"] = corkboardPos.x();
    obj["y"] = corkboardPos.y();

    QJsonObject casingJson;
    casingJson["idea"] = ideaJson;
    casingJson["id"] = QUuid::createUuid().toString();
    casingJson["name"] = name;
    casingJson["comp"] = obj;

    QJsonObject newWireJson;
    newWireJson["id"] = QUuid::createUuid().toString();
    newWireJson["i_id"] = casingJson["id"];
    newWireJson["i_n"] = 0;
    newWireJson["o_id"] = sourceID.toString();
    newWireJson["o_n"] = plug;

    AddDefaultDisplayCommand* command = new AddDefaultDisplayCommand(casingJson, newWireJson, this);
    m_undoStack->push(command);
}

void Corkboard::createIdeaDefaultSource(QString name, QUuid sinkID, PlugNumber plug, QPointF eventPos)
{
    deselectAll();
    QPointF corkboardPos =  mapFromScene(eventPos - QPointF(150, 20));

    QJsonObject ideaJson;

    QJsonObject obj;
    obj["x"] = corkboardPos.x();
    obj["y"] = corkboardPos.y();

    QJsonObject casingJson;
    casingJson["idea"] = ideaJson;
    casingJson["id"] = QUuid::createUuid().toString();
    casingJson["name"] = name;
    casingJson["comp"] = obj;

    QJsonObject newWireJson;
    newWireJson["id"] = QUuid::createUuid().toString();
    newWireJson["o_id"] = casingJson["id"];
    newWireJson["o_n"] = 0;
    newWireJson["i_id"] = sinkID.toString();
    newWireJson["i_n"] = plug;

    AddDefaultSourceCommand* command = new AddDefaultSourceCommand(casingJson, newWireJson, this);
    m_undoStack->push(command);
}

Wire *Corkboard::createWire(WireBackend* wireBackend)
{
    deselectAll();

    QQmlComponent component(qmlEngine(),"qrc:/QML/Wire.qml");
    QObject* preWire = component.create(m_rootContext);

    Wire* wire = qobject_cast<Wire*>(preWire);
    wire->setParentItem(this);
    wire->setParent(this);
    wire->setCorkboard(this);
    wire->setBackend(wireBackend);
    wireBackend->setWire(wire);

    return wire;
}

void Corkboard::createWire(const QJsonObject &jsonObj)
{
    deselectAll();
    AddWireCommand(jsonObj,this,false);
}

Wire *Corkboard::createWireBypassUndoStack(const QJsonObject &jsonObj)
{
    auto wireBackend = backend()->restoreWireBackend(jsonObj);

    QQmlComponent qmlComponent(qmlEngine(),"qrc:/QML/Wire.qml");
    QObject* preWire = qmlComponent.create(m_rootContext);

    Wire* wire = qobject_cast<Wire*>(preWire);
    wire->setParentItem(this);
    wire->setParent(this);
    wire->setCorkboard(this);
    wire->setBackend(wireBackend.get());
    wireBackend->setWire(wire);

    wire->finishSetup(m_dataTypeRegistry->dataTypeColor(wireBackend->dataType()));
    wireBackend->getCasing(PlugType::DataOut)->sendDataNewWire(wireBackend->id());

    return wire;
}

void Corkboard::shiftWire(const QJsonObject &oldWire, const QJsonObject &newWire)
{
    ShiftWireCommand* command = new ShiftWireCommand(oldWire,newWire,this,false);
    m_undoStack->push(command);
}

void Corkboard::changeParams(QUuid id, QJsonObject oldParams, QJsonObject newParams, bool actionNeeded)
{
    ChangeParamsCommand* command = new ChangeParamsCommand(id,oldParams,newParams,this, actionNeeded);
    m_undoStack->push(command);
}

void Corkboard::changeSize(QUuid id, QPointF oldSize, QPointF newSize)
{
    ResizeCasingCommand* command = new ResizeCasingCommand(id, this, oldSize, newSize);
    m_undoStack->push(command);
}

void Corkboard::createPostIt(QPointF pt, QString title, QString shortHelpMessage, QString longHelpMessage)
{
    QJsonObject json;
    json["id"] = QUuid::createUuid().toString();
    json["x"] = pt.x();
    json["y"] = pt.y();
    json["title"] = title;
    json["smsg"] = shortHelpMessage;
    json["lmsg"] = longHelpMessage;

    AddPostItCommand* command = new AddPostItCommand(json,this);
    m_undoStack->push(command);
}

void Corkboard::removePostIt(const QJsonObject &json)
{
    DeletePostItCommand* command = new DeletePostItCommand(json,this);
    m_undoStack->push(command);
}

void Corkboard::createPostItBypassUndoStack(const QJsonObject &json)
{
    QQmlComponent qmlComponent(qmlEngine(), QUrl(QStringLiteral("qrc:/QML/PostIt.qml")));
    QObject* prePostIt = qmlComponent.create(m_rootContext);

    PostIt* postIt = qobject_cast<PostIt*>(prePostIt);
    postIt->setCorkboard(this);
    postIt->load(json);
    postIt->setParent(this);
    postIt->setParentItem(this);

    m_postIts[postIt->id()] = postIt;
}

void Corkboard::removePostItBypassUndoStack(QUuid id)
{
    m_postIts[id]->deleteLater();
    m_postIts.erase(id);
}

void Corkboard::createOrShiftWire(const QJsonObject &jsonObj)
{
    if(m_wirePlugSwitch.isEmpty())
    {
        //this looks weird, because it is. The final parameter false just means don't do this the first time.
        // it actually works, just it's odd that it works.
        AddWireCommand* command = new AddWireCommand(jsonObj,this, false);
        m_undoStack->push(command);
    }
    else
    {
        if(jsonObj["i_id"] == m_wirePlugSwitch["i_id"] &&
                jsonObj["o_id"] == m_wirePlugSwitch["o_id"] &&
                jsonObj["i_n"] == m_wirePlugSwitch["i_n"] &&
                jsonObj["o_n"] == m_wirePlugSwitch["o_n"])
        {
            // Case when a wire was disconnected and then reconnected
            // to the same plug.
            // Since nothing happened, nothing needs to happen.
        }
        else
        {
            //this looks weird, because it is. The final parameter false just means don't do this the first time.
            // it actually works, just it's odd that it works.
            ShiftWireCommand* command = new ShiftWireCommand(m_wirePlugSwitch,jsonObj,this, false);
            m_undoStack->push(command);
        }
    }
}

bool Corkboard::destroyWire()
{
    if(m_wirePlugSwitch.isEmpty())
    {
        //No wire existed before this. So nothing needs to be done.
        return false;
    }
    else
    {
        //Case when a wire is destroyed by dragging off into the distance.
        DeleteWireCommand* command = new DeleteWireCommand(m_wirePlugSwitch,this);
        m_undoStack->push(command);
        return true;
    }
}

void Corkboard::moveItemFromCommand(QUuid id, QPointF pos)
{
    CasingBackend* casingBackend = backend()->getCasing(id);
    if(casingBackend)
    {
        casingBackend->casing().setX(pos.x());
        casingBackend->casing().setY(pos.y());
        casingBackend->moveWires();
        return;
    }
    PostIt* postIt = m_postIts[id];
    if(postIt)
    {
        postIt->setX(pos.x());
        postIt->setY(pos.y());
        return;
    }
    qFatal("Move called on invalid id");
}

void Corkboard::reportParamsChanged(QUuid id, QJsonObject oldParams, QJsonObject newParams)
{
    ChangeParamsCommand* command = new ChangeParamsCommand(id, oldParams, newParams, this, false);
    m_undoStack->push(command);
}

QVector<CasingBackend *> Corkboard::locateCasingNear(QPointF pos, int padding)
{
    QVector<CasingBackend*> casingBackends;

    QObjectList children = this->children();

    for(int i = 0; i < children.count(); i++)
    {
        Casing* casing = qobject_cast<Casing*>(children[i]);

        if(casing && casing->x() < pos.x() + padding && casing->y() < pos.y() + padding &&
                casing->x()+casing->width() + padding > pos.x() &&
                casing->y() + casing->height() + padding > pos.y())
        {
            casingBackends.append(casing->casingBackend());
        }
    }
    return casingBackends;
}

void Corkboard::deselectAll()
{
    foreach(QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing)
        {
            casing->setSelected(false);
            continue;
        }

        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire)
        {
            wire->setSelected(false);
            continue;
        }

        PostIt* postIt = qobject_cast<PostIt*>(obj);
        if(postIt)
        {
            postIt->setSelected(false);
            continue;
        }
    }
}

void Corkboard::selectAll()
{
    foreach(QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing)
        {
            casing->setSelected(true);
            continue;
        }

        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire)
        {
            wire->setSelected(true);
            continue;
        }

        PostIt* postIt = qobject_cast<PostIt*>(obj);
        if(postIt)
        {
            postIt->setSelected(true);
            continue;
        }
    }
}

QByteArray Corkboard::saveData()
{
    QJsonObject sceneJson = m_backend->saveToMemory();
    sceneJson["postIts"] = savePostIts();
    QJsonDocument doc(sceneJson);
    return doc.toJson(QJsonDocument::Compact);
}

void Corkboard::loadData(QByteArray data)
{
    m_undoStack->clear();
    QJsonObject const jsonDocument = QJsonDocument::fromJson(data).object();

    QJsonArray casingsJsonArray = jsonDocument["casings"].toArray();
    for (QJsonValueRef casing : casingsJsonArray)
    {
        createCasingBypassUndoStack(casing.toObject());
    }

    QJsonArray wiresJsonArray = jsonDocument["wires"].toArray();
    for (QJsonValueRef wire : wiresJsonArray)
    {
        createWireBypassUndoStack(wire.toObject());
    }

    QJsonArray postItsJsonArray = jsonDocument["postIts"].toArray();
    for(QJsonValueRef postIt : postItsJsonArray)
    {
        createPostItBypassUndoStack(postIt.toObject());
    }
}

void Corkboard::selectInBox(QRectF selectionBox)
{
    foreach (QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing)
        {
            double casingCenterX = casing->x() + casing->width() / 2;
            double casingCenterY = casing->y() + casing->height() / 2;

            if(casingCenterX > selectionBox.left() && casingCenterX < selectionBox.right() &&
                    casingCenterY > selectionBox.top() && casingCenterY < selectionBox.bottom())
            {
                casing->setSelected(true);
            }
            continue;
        }
        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire)
        {
            double wireCenterX = wire->x() + wire->width() / 2;
            double wireCenterY = wire->y() + wire->height() / 2;

            if(wireCenterX > selectionBox.left() && wireCenterX < selectionBox.right() &&
                    wireCenterY > selectionBox.top() && wireCenterY < selectionBox.bottom())
            {
                wire->setSelected(true);
            }
            continue;
        }
        PostIt* postIt = qobject_cast<PostIt*>(obj);
        if(postIt)
        {
            double postItCenterX = postIt->x() + postIt->width() / 2;
            double postItCenterY = postIt->y() + postIt->height() / 2;

            if(postItCenterX > selectionBox.left() && postItCenterX < selectionBox.right() &&
                    postItCenterY > selectionBox.top() && postItCenterY < selectionBox.bottom())
            {
                postIt->setSelected(true);
            }
            continue;
        }
    }
}

QObjectList Corkboard::selected()
{
    QObjectList list;

    foreach (QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing && casing->selected())
        {
            list.append(casing);
            continue;
        }
        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire && wire->selected())
        {
            list.append(wire);
            continue;
        }
        PostIt* postIt = qobject_cast<PostIt*>(obj);
        if(postIt && postIt->selected())
        {
            list.append(postIt);
            continue;
        }
    }
    return list;
}

std::vector<Casing *> Corkboard::selectedCasings()
{
    std::vector<Casing*> casings;
    foreach (QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing && casing->selected())
        {
            casings.push_back(casing);
        }

    }
    return casings;
}

std::vector<QQuickItem *> Corkboard::selectedNonWires()
{
    std::vector<QQuickItem*> selectedItems;
    foreach (QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing && casing->selected())
        {
            selectedItems.push_back(casing);
            continue;
        }
        PostIt* postIt = qobject_cast<PostIt*>(obj);
        if(postIt && postIt->selected())
        {
            selectedItems.push_back(postIt);
            continue;
        }

    }
    return selectedItems;
}

std::vector<Wire *> Corkboard::selectedWires()
{
    std::vector<Wire*> wires;
    foreach (QObject* obj, children())
    {
        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire && wire->selected())
        {
            wires.push_back(wire);
        }
    }
    return wires;
}

QJsonArray Corkboard::savePostIts()
{
    QJsonArray postItJsonArray;
    for(auto const & pair : m_postIts)
    {
        auto const & postIt = pair.second;
        QJsonObject postItJson = postIt->save();
        if(!postItJson.isEmpty())
            postItJsonArray.append(postItJson);
    }

    return postItJsonArray;
}

QJsonArray Corkboard::saveSelectedPostIts()
{
    QJsonArray postItJsonArray;
    for(auto const & pair : m_postIts)
    {
        auto const & postIt = pair.second;
        if(postIt->selected())
        {
            QJsonObject postItJson = postIt->save();
            if(!postItJson.isEmpty())
                postItJsonArray.append(postItJson);
        }
    }

    return postItJsonArray;
}

bool Corkboard::canUndo()
{
    return m_undoStack->canUndo();
}

void Corkboard::prepareMoveSelection()
{
    m_movingItems = selectedNonWires();
    m_initialPositions.reserve(m_movingItems.size());

    for(unsigned int i = 0; i < m_movingItems.size(); i++)
    {
        m_initialPositions.push_back(m_movingItems[i]->position());
    }
}

void Corkboard::moveSelection(QPointF displacement)
{
    QPointF scaledDisplacement(displacement.x() / scale(),
                               displacement.y() / scale());


    for(unsigned int i = 0; i < m_movingItems.size(); i++)
    {
        m_movingItems[i]->setPosition(m_initialPositions[i] + scaledDisplacement);
        Casing* casing = qobject_cast<Casing*>(m_movingItems[i]);
        if(casing)
            casing->casingBackend()->moveWires();
    }
}

void Corkboard::finishMoveSelection()
{
    //This sets up the undo/redo stack to do the rigth things.
    std::vector<QPointF> finalPositions;
    std::vector<QUuid> idList;

    unsigned int size = static_cast<unsigned int>(m_movingItems.size());
    finalPositions.reserve(size);
    idList.reserve(size);

    for(unsigned int i = 0; i < size; i++)
    {
        Casing* casing = qobject_cast<Casing*>(m_movingItems[i]);
        if(casing)
        {
            casing->casingBackend()->moveWires();
            idList.push_back(casing->casingBackend()->id());
            finalPositions.push_back(m_movingItems[i]->position());
            continue;
        }
        PostIt* postIt = qobject_cast<PostIt*>(m_movingItems[i]);
        if(postIt)
        {
            idList.push_back(postIt->id());
            finalPositions.push_back(m_movingItems[i]->position());
            continue;
        }
    }

    //Ensure a move actually happened.
    if(qAbs(m_initialPositions[0].x() - finalPositions[0].x()) > 0.00001
            || qAbs(m_initialPositions[0].y() - finalPositions[0].y()) > 0.00001)
    {
        MoveSelectionCommand* command = new MoveSelectionCommand(idList, m_initialPositions, finalPositions, this, false);
        m_undoStack->push(command);
    }

    m_initialPositions.clear();
}

void Corkboard::resizeCasingFromCommand(QUuid id, QPointF size)
{
    CasingBackend* wrap = backend()->getCasing(id);

    if(!wrap->casing().embeddedQML())
        return;

    QQuickItem* embedded = wrap->getEmbeddedQML();
    embedded->setWidth(size.x());
    embedded->setHeight(size.y());
}

void Corkboard::reportSizeChanged(QUuid id, QPointF oldSize, QPointF newSize)
{
    //Ensure a resize actually happened.
    if(qAbs(oldSize.x() - newSize.x()) > 0.00001
            || qAbs(oldSize.y() - newSize.y()) > 0.00001)
    {
        ResizeCasingCommand* command = new ResizeCasingCommand(id, this, oldSize, newSize);
        m_undoStack->push(command);
    }
}

void Corkboard::reportIdeaMinized(QUuid id, bool oldState)
{
    MinimizeCasingCommand* command = new MinimizeCasingCommand(id, this, oldState);
    m_undoStack->push(command);
}

void Corkboard::minimizeCasingFromCommand(QUuid id, bool state)
{
    CasingBackend* wrap = backend()->getCasing(id);
    wrap->casing().setMinimized(state);
}

void Corkboard::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
        zoomIn(event->position());
    else
        zoomOut(event->position());
}

void Corkboard::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy))
        copy();
    else if(event->matches(QKeySequence::Cut))
        cut();
    else if(event->matches(QKeySequence::Paste))
        paste();
    else if(event->matches(QKeySequence::Undo))
        undo();
    else if(event->matches(QKeySequence::Redo))
        redo();
    else if(event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_D)
    {
        duplicate();
    }
    else if(event->matches(QKeySequence::SelectAll))
        selectAll();

    if(event->key() ==  Qt::Key_Delete)
        deleteSelection();
    else if(event->key() == Qt::Key_Left)
        setX(x() + 100);
    else if(event->key() == Qt::Key_Right)
        setX(x() - 100);
    else if(event->key() == Qt::Key_Up)
        setY(y() + 100);
    else if(event->key() == Qt::Key_Down)
        setY(y() - 100);
}
