#include "Casing.h"

#include <iostream>
#include <cstdlib>

#include <QtMath>
#include <QtWidgets>
#include <QGraphicsEffect>
#include <QJsonValue>

#include "Wire.h"
#include "WireState.h"
#include "CorkboardBackend.h"
#include "Corkboard.h"
#include "CasingBackend.h"
#include "WireCasingInteraction.h"

Casing::Casing(QQuickItem *parent) : QQuickItem(parent),
    m_moving(false),
    m_resizing(false),

    m_QMLBoxX(0),
    m_QMLBoxY(0),
    m_QMLBoxWidth(0),
    m_QMLBoxHeight(0),

    m_predefinedColor(0),

    m_selected(true),
    m_minimized(false),
    m_minimizedSetDuringLoad(false),
    m_changeNameOnMinimized(false)
{    
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setCursor(QCursor(QPixmap(":/Images/StylizedCursor2.png"),2,2));
    setAntialiasing(true);
    setZ(1);
}

Casing::~Casing()
{

}

QJsonObject Casing::save() const
{
    QJsonObject saveJson;
    saveJson["x"] = x();
    saveJson["y"] = y();
    saveJson["c"] = predefinedColor();
    if(m_minimizable)
    {
        saveJson["m"] = m_minimized;
    }
    return saveJson;
}

void Casing::load(const QJsonObject &json)
{
    QJsonValue x = json["x"];
    if(!x.isUndefined())
    {
        setX(x.toDouble());
    }

    QJsonValue y = json["y"];
    if(!y.isUndefined())
    {
        setY(y.toDouble());
    }

    QJsonValue c = json["c"];
    if(!c.isUndefined())
    {
        setPredefinedColor(c.toInt());
    }

    QJsonValue m = json["m"];
    if(!m.isUndefined())
    {
        m_minimized = m.toBool();
        m_minimizedSetDuringLoad = true;
    }
}
void Casing::finishSetup()
{
    IdeaAttributes attributes = m_backend->idea()->attributes();
    m_hasEmbeddedQML = attributes.embeddedQMLPath() == "" ? false : true;
    m_hasMinimizedQML = attributes.minimizedQMLPath() == "" ? false : true;
    m_changeNameOnMinimized = attributes.differentNameOnMinimized();
    setHasExtraQML(attributes.externalQMLPaths().size() == 0 ? false : true);

    m_resizeable = attributes.resizeable();
    emit resizeableChanged(m_resizeable);
    m_minimizable = attributes.minimizable();
    emit minimizableChanged(m_minimizable);

    if(m_minimizable && !m_minimizedSetDuringLoad)
        m_minimized = attributes.startMinimized();

    setupInPlugs();
    setupOutPlugs();
    setupCaption();

    m_backend->clearStopLights();

    if(m_minimizable == false && m_hasEmbeddedQML == true)
    {
        m_backend->loadEmbeddedQML();
    }
    else if(m_minimizable == true && m_minimized == false && m_hasEmbeddedQML == true)
    {
        m_backend->loadEmbeddedQML();
    }
    else if(m_minimizable == true && m_minimized == true && m_hasMinimizedQML == true)
    {
        m_backend->loadMinimizedQML();
    }

    setupValidationBox();
}

void Casing::setupCaption()
{
    IdeaAttributes attributes = m_backend->idea()->attributes();

    if(attributes.displayName() == "")
    {
        m_nameSize = QSize();
        emit nameSizeChanged(m_nameSize);
        m_ideaName = "";
        emit ideaNameChanged(m_ideaName);
    }
    else if(!m_minimized || !m_changeNameOnMinimized)
    {
        m_ideaName = attributes.displayName();
        emit ideaNameChanged(m_ideaName);

        QFont nameFont("consolas",12);
        nameFont.setBold(true);
        QFontMetrics nameFontMetrics(nameFont);
        m_nameSize = nameFontMetrics.size(Qt::TextShowMnemonic,ideaName());
        emit nameSizeChanged(m_nameSize);
    }
    else /// Minimized and name different on minimized
    {
        m_ideaName = attributes.minimizedName();
        emit ideaNameChanged(m_ideaName);

        QFont nameFont("consolas",12);
        nameFont.setBold(true);
        QFontMetrics nameFontMetrics(nameFont);
        m_nameSize = nameFontMetrics.size(Qt::TextShowMnemonic,ideaName());
        emit nameSizeChanged(m_nameSize);
    }
}

void Casing::setupInPlugs()
{
    IdeaAttributes attributes = m_backend->idea()->attributes();

    //Setup the sink plugs
    m_inPlugs = static_cast<int> (attributes.dataTypesIn().size());
    int labelWidth = 0;
    for(int i = 0; i < m_inPlugs; i++)
    {
        m_inPlugColors.append(m_corkboard->dataTypeRegistry()->dataTypeColor(attributes.dataTypesIn()[i]));

        QString label;
        if(attributes.customLabels())
            label = attributes.labelsIn()[i];
        else
            label = m_corkboard->dataTypeRegistry()->dataTypeAbbreviation(attributes.dataTypesIn()[i]);

        QFont labelFont("consolas",11);
        QFontMetrics fm(labelFont);

        if(fm.horizontalAdvance(label) >= labelWidth)
            labelWidth = fm.horizontalAdvance(label);

        m_inPlugLabels.append(label);
    }

    setInLabelWidth(labelWidth);
    emit inPlugLabelsChanged(m_inPlugLabels);
}

void Casing::setupOutPlugs()
{
    IdeaAttributes attributes = m_backend->idea()->attributes();

    //Setup the source plugs
    m_outPlugs = static_cast<int> (attributes.dataTypesOut().size());
    int labelWidth = 0;
    for(int i = 0; i < m_outPlugs; i++)
    {
        m_outPlugColors.append(m_corkboard->dataTypeRegistry()->dataTypeColor(attributes.dataTypesOut()[i]));

        QString label;
        if(attributes.customLabels())
            label = attributes.labelsOut()[i];
        else
            label = m_corkboard->dataTypeRegistry()->dataTypeAbbreviation(attributes.dataTypesOut()[i]);

        QFont labelFont("consolas",11);
        QFontMetrics fm(labelFont);

        if(fm.horizontalAdvance(label) >= labelWidth)
            labelWidth = fm.horizontalAdvance(label);

        m_outPlugLabels.append(label);
    }
    setOutLabelWidth(labelWidth);
    emit outPlugLabelsChanged(m_outPlugLabels);
}

void Casing::setupValidationBox()
{
    setValidationMessage(m_backend->idea()->shortStatusMessage());

    QFont statusFont("consolas",12);
    statusFont.setBold(true);
    QFontMetrics statusFontMetrics(statusFont);
    QSize statusSize = statusFontMetrics.size(Qt::TextShowMnemonic,validationMessage());
    setValidationBoxSize(statusSize);

    //Setup the validation area.
    IdeaStatus validationStatus = m_backend->idea()->status();

    switch (validationStatus)
    {
    case IdeaStatus::Working:
        setValidationColor(QColor("darkgreen"));
        setValidationMessage("");
        setValidationBoxSize(QSize(0,0));
        break;
    case IdeaStatus::StandBy:
        setValidationColor(QColor("#62aeef"));
        break;
    case IdeaStatus::Warning:
        setValidationColor(QColor("#78ae51"));
        break;
    case IdeaStatus::DisplayError:
        setValidationColor(QColor("#ae9451"));
        break;
    case IdeaStatus::Error:
        setValidationColor(QColor("#f64f4f"));
        break;
    case IdeaStatus::Fatal:
        setValidationColor(QColor("black"));
        break;
    }

    recalculateSize();
}

Corkboard *Casing::corkboard() const
{
    return m_corkboard;
}

CasingBackend* Casing::casingBackend()
{
    return m_backend;
}

void Casing::setCorkboard(Corkboard *corkboard)
{
    m_corkboard = corkboard;
    setParentItem(corkboard);
    setParent(corkboard);
}

void Casing::setBackend(CasingBackend *backend)
{
    m_backend = backend;
}

void Casing::mousePressEvent(QMouseEvent *event)
{
    setFocus(true);
    if(event->buttons() == Qt::RightButton)
    {
        m_corkboard->readyCasingMenu(this, mapToScene(event->pos()));
        event->accept();
        return;
    }

    m_backend->onDeleteExternalQML();

    //Check if creating/editing a wire.
    for (PlugType plugToCheck: {PlugType::DataIn, PlugType::DataOut})
    {
        int const plugIndex = checkHitScenePoint(plugToCheck,event->pos());

        if(plugIndex != INVALID)
        {
            //Clicked on Valid plug. If there is an existing wire drag it. If no wire create one.
            CasingState const & state = m_backend->casingState();
            std::unordered_map<QUuid,WireBackend*> wires = state.wires(plugToCheck,plugIndex);

            //Drag existing wire.
            if(!wires.empty() && plugToCheck == PlugType::DataIn)
            {
                auto wire = wires.begin()->second;
                wire->propagateEmptyData();

                //This stores the previous state for the undo/redo command framework.
                m_corkboard->setWirePlugSwitch(wire->serialize());

                WireCasingInteraction interaction(*m_backend,*wire);
                interaction.disconnect(plugToCheck);

                event->accept();
                wire->getWire()->simulateMousePressEvent(event);
                wire->getWire()->grabMouse();
                return;
            }

            else //Create new wire or default display or default source
            {
                if(event->modifiers() & Qt::ControlModifier) //Create a default display or source.;
                {
                    if(plugToCheck == PlugType::DataOut)
                    {
                        int dataType = m_backend->idea()->attributes().dataTypesOut()[plugIndex];
                        QString defaultDisplay = m_corkboard->dataTypeRegistry()->defaultDisplay(dataType);
                        m_corkboard->createIdeaDefaultDisplay(defaultDisplay, m_backend->id(),
                                                              plugIndex, event->windowPos());
                    }
                    else
                    {
                        int dataType = m_backend->idea()->attributes().dataTypesIn()[plugIndex];
                        QString defaultSource = m_corkboard->dataTypeRegistry()->defaultSource(dataType);
                        m_corkboard->createIdeaDefaultSource(defaultSource, m_backend->id(),
                                                             plugIndex, event->windowPos());
                    }
                    return;
                }

                //This lets the undo/redo framework know that there wasn't a previous wire.
                m_corkboard->setWirePlugSwitch(QJsonObject());

                auto wireBackend = m_corkboard->backend()->createWireBackend(plugToCheck,m_backend,plugIndex);
                Wire* wire = m_corkboard->createWire(wireBackend.get());
                wire->setColor(m_corkboard->dataTypeRegistry()->dataTypeColor(wireBackend->dataType()));
                m_backend->casingState().setWire(plugToCheck,plugIndex,*wireBackend);

                event->accept();
                wireBackend->getWire()->simulateMousePressEvent(event);
                wireBackend->getWire()->grabMouse();
                return;
            }
        }
    }

    if (!(event->modifiers() & Qt::ShiftModifier))
    {
        if(selected() == false)
            m_corkboard->deselectAll();

        setSelected(true);
    }
    else
    {
        setSelected(!selected());
        event->accept();
        return;
    }

    //Check if mouse is over the resizer.
    if(resizeable() && checkHitResizer(event->pos()))
    {
        QQuickItem* embed = m_backend->getEmbeddedQML();
        initialClickPos = event->windowPos();
        initialSize = QPointF(embed->width(), embed->height());
        m_resizing = true;
    }
    else
    {
        //Assume the begining of a drag event.
        initialClickPos = event->windowPos();
        m_moving = true;
        m_backend->setEmbeddedQMLEnabled(false);
        m_corkboard->prepareMoveSelection();
    }
    event->accept();
}

void Casing::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moving && event->buttons() & Qt::LeftButton)
    {
        QPointF displacement = event->windowPos() - initialClickPos;
        m_corkboard->moveSelection(displacement);
    }

    if(m_resizing && event->buttons() & Qt::LeftButton)
    {
        QPointF displacement = (event->windowPos() - initialClickPos) / corkboard()->scale();
        //The corkboard can be scaled. (Zoom in/Zoom out). The scale affects the children of the corkboard, aka the casing too.
        // So the displacement needs to be scaled. If this wasn't here the resizing and mouse move at different rates.
        QQuickItem* embed = m_backend->getEmbeddedQML();
        embed->setWidth(initialSize.x() + displacement.x());
        embed->setHeight(initialSize.y() + displacement.y());

        m_backend->moveWires();
    }
    event->accept();
}

void Casing::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_moving)
    {
        m_corkboard->finishMoveSelection();
        m_backend->moveWires();
        m_moving = false;
        m_backend->setEmbeddedQMLEnabled(true);
    }
    if(m_resizing)
    {
        QPointF displacement = (event->windowPos() - initialClickPos) / corkboard()->scale();
        QQuickItem* embed = m_backend->getEmbeddedQML();
        embed->setWidth(initialSize.x() + displacement.x());
        embed->setHeight(initialSize.y() + displacement.y());

        m_resizing = false;
        m_corkboard->reportSizeChanged(casingBackend()->id(), initialSize, QPointF(embed->width(),embed->height()));
    }
    event->accept();
}

void Casing::mouseDoubleClickEvent(QMouseEvent *event)
{    
    if(m_backend->idea()->status() != IdeaStatus::Working)
    {
        if(m_casingSize.height() - event->pos().y() < 14 + validationBoxSize().height())
        {
            m_backend->createPostIt();
            return;
        }
    }

    if(m_backend->getEmbeddedQMLLoaded())
    {
        m_backend->getEmbeddedQML()->setFocus(true);
    }
    else if(m_backend->getMinimizedQMLLoaded())
    {
        m_backend->getMinimizedQML()->setFocus(true);
    }
    event->accept();
}

bool Casing::selected() const
{
    return m_selected;
}

void Casing::setSelected(bool selected)
{
    if (m_selected == selected)
        return;

    update();
    m_selected = selected;
    emit selectedChanged(m_selected);
}

void Casing::setValidationColor(QColor validationColor)
{
    if (m_validationColor == validationColor)
        return;

    m_validationColor = validationColor;
    emit validationColorChanged(m_validationColor);
}

void Casing::setValidationMessage(QString validationMessage)
{
    if (m_validationMessage == validationMessage)
        return;

    m_validationMessage = validationMessage;
    emit validationMessageChanged(m_validationMessage);
}

void Casing::setInLabelWidth(int inLabelWidth)
{
    if (m_inLabelWidth == inLabelWidth)
        return;

    m_inLabelWidth = inLabelWidth;
    emit inLabelWidthChanged(m_inLabelWidth);
}

void Casing::setOutLabelWidth(int outLabelWidth)
{
    if (m_outLabelWidth == outLabelWidth)
        return;

    m_outLabelWidth = outLabelWidth;
    emit outLabelWidthChanged(m_outLabelWidth);
}

void Casing::setValidationBoxTop(int validationBoxTop)
{
    if (m_validationBoxTop == validationBoxTop)
        return;

    m_validationBoxTop = validationBoxTop;
    emit validationBoxTopChanged(m_validationBoxTop);
}

void Casing::setPlugBoxTop(int plugBoxTop)
{
    if (m_plugBoxTop == plugBoxTop)
        return;

    m_plugBoxTop = plugBoxTop;
    emit plugBoxTopChanged(m_plugBoxTop);
}

QString Casing::ideaName() const
{
    return m_ideaName;
}

QColor Casing::validationColor() const
{
    return m_validationColor;
}

QString Casing::validationMessage() const
{
    return m_validationMessage;
}

int Casing::inLabelWidth() const
{
    return m_inLabelWidth;
}

int Casing::outLabelWidth() const
{
    return m_outLabelWidth;
}

int Casing::validationBoxTop() const
{
    return m_validationBoxTop;
}

bool Casing::embeddedQML() const
{
    return m_hasEmbeddedQML;
}

int Casing::plugBoxTop() const
{
    return m_plugBoxTop;
}

bool Casing::resizeable() const
{
    return m_resizeable;
}

bool Casing::minimizable() const
{
    return m_minimizable;
}

bool Casing::minimized() const
{
    return m_minimized;
}

bool Casing::hasExtraQML() const
{
    return m_hasExtraQML;
}

int Casing::predefinedColor() const
{
    return m_predefinedColor;
}

QSize Casing::casingSize() const
{
    return m_casingSize;
}

QSize Casing::nameSize() const
{
    return m_nameSize;
}

QSize Casing::validationBoxSize() const
{
    return m_validationBoxSize;
}

QStringList Casing::inPlugLabels() const
{
    return m_inPlugLabels;
}

QStringList Casing::outPlugLabels() const
{
    return m_outPlugLabels;
}

void Casing::recalculateSize()
{
    int qmlHeight = 0;
    int qmlWidth = 0;
    int borderWidth = 4;

    if(m_backend->getEmbeddedQMLLoaded())
    {
        QQuickItem* qmlItem = m_backend->getEmbeddedQML();
        if(qmlItem)
        {
            qmlHeight = static_cast<int>(qmlItem->height() + 1);
            qmlWidth = static_cast<int>(qmlItem->width() + 1);
        }
    }
    else if(m_backend->getMinimizedQMLLoaded())
    {
        QQuickItem* qmlItem = m_backend->getMinimizedQML();
        if(qmlItem)
        {
            qmlHeight = static_cast<int>(qmlItem->height() + 1);
            qmlWidth = static_cast<int>(qmlItem->width() + 1);
        }
    }

    int casingHeight = borderWidth + 5;
    if(m_ideaName != QString())
        casingHeight += 5 + nameSize().height();

    setPlugBoxTop(casingHeight);
    m_QMLBoxY = casingHeight;

    int plugHeight;
    int plugNum = qMax(m_inPlugLabels.length(), m_outPlugLabels.length());
    if(plugNum == 0)
        plugHeight = 0;
    else
    {
        plugHeight = 15*plugNum + 7*(plugNum-1);
    }

    if(plugHeight > qmlHeight)
    {
        m_QMLBoxHeight = plugHeight;
        setPlugBoxTop(casingHeight);
        casingHeight += plugHeight;
    }
    else
    {
        m_QMLBoxHeight = qmlHeight;
        setPlugBoxTop(casingHeight + static_cast<int>(0.5*(qmlHeight - plugHeight)));
        casingHeight += qmlHeight;
    }

    casingHeight += 5;
    casingHeight += borderWidth;
    setValidationBoxTop(casingHeight);

    if(validationBoxSize().height() != 0)
        casingHeight += borderWidth + 2 * 5 + validationBoxSize().height();

    int totalCaptionWidth = nameSize().width() + 8 + 2*5;
    int totalValidationWidth = validationBoxSize().width() + 8 + 2*5;

    int totalPlugWidth = 0;

    if(m_inPlugs != 0)
        totalPlugWidth += 2 + 0.5* 15;
    else
        totalPlugWidth += 4;

    totalPlugWidth += 5;
    if(inLabelWidth() != 0)
        totalPlugWidth += 5 + inLabelWidth();

    totalPlugWidth += 5;
    m_QMLBoxX = totalPlugWidth;
    totalPlugWidth += qmlWidth;
    totalPlugWidth += 5;

    if(outLabelWidth() != 0)
        totalPlugWidth += 5 + outLabelWidth();

    totalPlugWidth += 5;
    if(m_outPlugs != 0)
        totalPlugWidth += 2 +0.5*15;
    else
        totalPlugWidth += 4;

    int casingWidth = qMax(totalCaptionWidth,totalValidationWidth);
    if(totalPlugWidth > casingWidth)
    {
        m_QMLBoxWidth = qmlWidth;
        setCasingSize(QSize(totalPlugWidth,casingHeight));
    }
    else
    {
        m_QMLBoxWidth = qmlWidth - totalPlugWidth + casingWidth;
        setCasingSize(QSize(casingWidth,casingHeight));
    }


    if(m_hasEmbeddedQML || m_hasMinimizedQML)
        repositionQML();

    m_backend->moveWires();
}

QPointF Casing::plugScenePosition(PlugNumber index, PlugType plugType) const
{
    QPointF result;

    float totalHeight = 0.2*15; // Top of Casing Rect.

    totalHeight += plugBoxTop();
    totalHeight += (5 + 15) * index;
    totalHeight += 15/2.0;

    switch (plugType)
    {
    case PlugType::DataOut:
    {
        double x = casingSize().width() + 0.7* 15 - 2;

        result = QPointF(x, totalHeight);
        break;
    }

    case PlugType::DataIn:
    {
        double x = 0.7* 15 + 2;

        result = QPointF(x, totalHeight);
        break;
    }

    default:
        break;
    }

    return result;
}

PlugNumber Casing::checkHitScenePoint(PlugType plugType, QPointF const point) const
{
    PlugNumber result = INVALID;

    if (plugType == PlugType::None)
        return result;

    double const yTolerance = 1.2 * 15;
    double const xTolerance = 1.2 * 15;

    unsigned int nItems;
    if(plugType == PlugType::DataIn)
        nItems = m_inPlugs;
    else
        nItems = m_outPlugs;

    for (unsigned int i = 0; i < nItems; ++i)
    {
        auto pp = plugScenePosition(i, plugType);

        QPointF p = pp - point;

        double xFactor = p.x() / xTolerance;
        double yFactor = p.y() / yTolerance;

        if(xFactor < 1.0 && xFactor > -1.0)
        {
            if(yFactor < 1.0 && yFactor > -1.0)
            {
                result = PlugNumber(i);
                break;
            }
        }
    }
    return result;
}

PlugNumber Casing::checkHitScenePointHighTolerance(PlugType plugType, QPointF point) const
{
    PlugNumber result = INVALID;

    if (plugType == PlugType::None)
        return result;

    double const yTolerance = 1.2 * 15;
    double const xTolerance = 1.2 * 15;

    unsigned int nItems;
    if(plugType == PlugType::DataIn)
        nItems = m_inPlugs;
    else
        nItems = m_outPlugs;

    for (unsigned int i = 0; i < nItems; ++i)
    {
        auto pp = plugScenePosition(i, plugType);

        QPointF p = pp - point;

        double xFactor = p.x() / xTolerance;
        double yFactor = p.y() / yTolerance;

        if(plugType == PlugType::DataIn)
        {
            if(xFactor < 3.0 && xFactor > -1.0)
            {
                if(yFactor < 1.0 && yFactor > -1.0)
                {
                    result = PlugNumber(i);
                    break;
                }
            }
        }
        if(plugType == PlugType::DataOut)
        {
            if(xFactor < 1.0 && xFactor > -3.0)
            {
                if(yFactor < 1.0 && yFactor > -1.0)
                {
                    result = PlugNumber(i);
                    break;
                }
            }
        }

    }
    return result;
}

bool Casing::checkHitResizer(QPointF point) const
{
    QPointF resizerPosition = QPointF(0.7*15 + casingSize().width() - 5 - 10.0,
                                      0.2*15 + casingSize().height() - 5 - 10.0);

    QPointF p = resizerPosition - point;
    auto distance = std::sqrt(QPointF::dotProduct(p,p));
    if(distance < 10)
        return true;
    else
        return false;
}

void Casing::repositionQML() const
{
    QQuickItem* qmlItem;
    if(m_backend->getEmbeddedQMLLoaded())
    {
        qmlItem = m_backend->getEmbeddedQML();
    }
    else if(m_backend->getMinimizedQMLLoaded())
    {
        qmlItem = m_backend->getMinimizedQML();
    }
    else
    {
        //There is no guarentee that the QML is loaded before this method is called.
        return;
    }
    int qmlHeight = static_cast<int>(qmlItem->height() + 0.5);
    int qmlWidth = static_cast<int>(qmlItem->width() + 0.5);

    int xPos = static_cast<int>(0.7*15) + m_QMLBoxX;
    if(m_QMLBoxWidth > qmlWidth)
        xPos += (m_QMLBoxWidth - qmlWidth) / 2;
    qmlItem->setX(xPos);

    int yPos = static_cast<int>(0.2*15) + m_QMLBoxY;
    if(m_QMLBoxHeight > qmlHeight)
        yPos += (m_QMLBoxHeight - qmlHeight) / 2;
    qmlItem->setY(yPos);
}

QColor Casing::inPlugColor(int plug)
{
    return m_inPlugColors.at(plug);
}

QColor Casing::outPlugColor(int plug)
{
    return m_outPlugColors.at(plug);
}

void Casing::minimizeButtonClicked()
{
    m_backend->onDeleteExternalQML();
    m_corkboard->reportIdeaMinized(m_backend->id(),m_minimized);
    setMinimized(!m_minimized);
}

void Casing::setMinimized(bool minimized)
{
    if (m_minimized == minimized)
        return;

    if(!m_minimizable)
        return;


    m_minimized = minimized;

    emit minimizedChanged(m_minimized);
    if(minimized)
    {
        casingBackend()->deleteEmbeddedQML();
        if(m_hasMinimizedQML)
            m_backend->loadMinimizedQML();
    }
    else
    {
        if(m_hasMinimizedQML)
            m_backend->deleteMinimizedQML();
        casingBackend()->loadEmbeddedQML();
    }

    if(m_changeNameOnMinimized)
        setupCaption();
    m_backend->idea()->resetDisplay();
    recalculateSize();
}

void Casing::extraButtonClicked()
{
    if(!m_backend->getExternalQMLLoaded())
        m_backend->loadExternalQML();
    else
        m_backend->onDeleteExternalQML();
}

void Casing::setHasExtraQML(bool hasExtraQML)
{
    if (m_hasExtraQML == hasExtraQML)
        return;

    m_hasExtraQML = hasExtraQML;
    emit hasExtraQMLChanged(m_hasExtraQML);
}

void Casing::setPredefinedColor(int predefinedColor)
{
    if (m_predefinedColor == predefinedColor)
        return;

    m_predefinedColor = predefinedColor;
    emit predefinedColorChanged(m_predefinedColor);
}

void Casing::setValidationBoxSize(QSize validationBoxSize)
{
    if (m_validationBoxSize == validationBoxSize)
        return;

    m_validationBoxSize = validationBoxSize;
    emit validationBoxSizeChanged(m_validationBoxSize);
}

void Casing::setCasingSize(QSize casingSize)
{
    if (m_casingSize == casingSize)
        return;

    m_casingSize = casingSize;
    emit casingSizeChanged(m_casingSize);
}

void Casing::hoverEnterEvent(QHoverEvent *event)
{
    hoverEvents(event);
}

void Casing::hoverMoveEvent(QHoverEvent *event)
{
    hoverEvents(event);
}

void Casing::hoverEvents(QHoverEvent *event)
{
    //Check if creating/editing a wire.
    int const plugOutIndex = checkHitScenePoint(PlugType::DataOut, event->pos());

    //Creating a new plug.
    if(plugOutIndex != INVALID)
    {
        setCursor(QCursor(QPixmap(":/Images/CreateWireRight.png")));
        event->accept();
        return;
    }

    int const plugInIndex = checkHitScenePoint(PlugType::DataIn, event->pos());


    if(plugInIndex != INVALID)
    {
        //Clicked on Valid plug. If there is an existing wire signal to drag it. If no wire create one.
        CasingState const & state = m_backend->casingState();
        std::unordered_map<QUuid,WireBackend*> wires = state.wires(PlugType::DataIn,plugInIndex);

        //Drag existing wire.
        if(!wires.empty())
        {
            setCursor(QCursor(QPixmap(":/Images/DisconnectWire.png")));
            event->accept();
            return;
        }
        else
        {
            setCursor(QCursor(QPixmap(":/Images/CreateWireLeft.png")));
            event->accept();
            return;
        }
    }
    setCursor(QCursor(QPixmap(":/Images/StylizedCursor2.png"),2,2));
}
