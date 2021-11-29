#include "Wire.h"

#include <QMouseEvent>
#include <QtMath>
#include <QCursor>
#include <QGuiApplication>

#include "WireBackend.h"
#include "CorkboardBackend.h"
#include "WireCasingInteraction.h"
#include "Corkboard.h"

Wire::Wire(QQuickItem *parent) : QQuickItem(parent),
    m_selected(false),
    m_transmitting(false),
    m_inPoint(0, 0),
    m_outPoint(0, 0),
    m_color(QColor("black")),
    m_validConnection(true)
{
    //An initial height is required for some reason. I don't get it. -dBob
    setWidth(2);
    setHeight(2);
    m_dragging = false;
    setCursor(QCursor(QPixmap(":/Images/StylizedCursor2.png"),2,2));
}

Wire::~Wire()
{

}

void Wire::finishSetup(QColor color)
{
    setColor(color);
    move();
}

void Wire::move()
{
    auto outCasingBackend = m_backend->getCasing(PlugType::DataOut);
    auto inCasingBackend = m_backend->getCasing(PlugType::DataIn);

    if(inCasingBackend && outCasingBackend)
    {
        auto const &inCasing = inCasingBackend->casing();
        auto const &outCasing = outCasingBackend->casing();

        QPointF inPosCasing = inCasing.plugScenePosition(m_backend->getPlugIndex(PlugType::DataIn),PlugType::DataIn);
        QPointF outPosCasing = outCasing.plugScenePosition(m_backend->getPlugIndex(PlugType::DataOut),PlugType::DataOut);

        QPointF inPos_Scene = QPointF(inPosCasing.x() + inCasing.x(), inPosCasing.y() + inCasing.y());
        QPointF outPos_Scene = QPointF(outPosCasing.x() + outCasing.x(), outPosCasing.y() + outCasing.y());

        setEndPoint(PlugType::DataOut, QPointF(outPos_Scene.x()-x(),outPos_Scene.y()-y()));
        setEndPoint(PlugType::DataIn, QPointF(inPos_Scene.x()-x(),inPos_Scene.y()-y()));
        updateControlPoints();
        emit inPointChanged(m_inPoint);
        emit outPointChanged(m_outPoint);
    }
}

//Based on the way this program is implemented, this happens in an usual way.
// The way to start sending mouse events to a wire is as follows:
// (1) Click on the Casing.
// (2) If the mouse is over a plug the Casing handles finding the correct
// way to generate a wire, move a wire, etc.
// (3) After identifying the correct way, the Casing hands control over to the correct wire
// via the grabMouse() method.
// (4) The wire then takes the mouseMoveEvent() calls.
//
// This skips the wire::mousePressEvent() method. Since I need the functionality of a
// wire::mousePressEvent(), I created this work-around. It's called before grabMouse()
// and sets up things like a mouse press event.
// it works! -dBob

void Wire::simulateMousePressEvent(QMouseEvent *event)
{
    auto requiredPlug = m_backend->requiredPlugType();
    Q_ASSERT(requiredPlug != PlugType::None);

    setDragging(true);
    setSelected(false);
    //Get the initial screen position of the click. This doesn't work the normal way
    //because the event was passed from a different object.
    m_initialScreenPosition = event->windowPos();

    auto currentPlug = oppositePlug(requiredPlug);
    auto const &curCasing = m_backend->getCasing(currentPlug)->casing();
    QPointF plugPositionOnCasing = curCasing.plugScenePosition(m_backend->getPlugIndex(currentPlug),currentPlug);
    m_initialPlugPosition = QPointF(plugPositionOnCasing.x() + curCasing.x(),
                                   plugPositionOnCasing.y() + curCasing.y());

    m_initialDisplacement = (m_inPoint - m_outPoint);

    m_corkboard->setEnabled(false);
}

void Wire::mouseMoveEvent(QMouseEvent *event)
{
    auto requiredPlug = m_backend->requiredPlugType();

    //The drag event.
    m_currentScreenPosition = event->windowPos();;

    QPointF totalDisplacement = m_initialDisplacement + m_currentScreenPosition - m_initialScreenPosition;
    QPointF scaledDisplacement = totalDisplacement / corkboard()->scale();
    setBoundingRect(m_initialPlugPosition,scaledDisplacement);

    QPointF outPlugPos;
    QPointF inPlugPos;

    if(requiredPlug == PlugType::DataIn)
    {
        outPlugPos = QPointF(m_initialPlugPosition.x() - x(), m_initialPlugPosition.y() - y());
        inPlugPos = outPlugPos + scaledDisplacement;
    }
    else
    {
        inPlugPos = QPointF(m_initialPlugPosition.x() - x(), m_initialPlugPosition.y() - y());
        outPlugPos = inPlugPos + scaledDisplacement;
    }
    setEndPoint(PlugType::DataOut, outPlugPos);
    setEndPoint(PlugType::DataIn, inPlugPos);
    updateControlPoints();
    emit inPointChanged(m_inPoint);
    emit outPointChanged(m_outPoint);

    //This code chunk sets up visual indicators to show if a wire can be created.

    //Identify the search position in corkboard coordinates.
    QPointF searchPoint = m_initialPlugPosition + scaledDisplacement;

    //Search for a possible casing.
    auto casingVector = m_corkboard->locateCasingNear(searchPoint);

    bool possibleConnection = false;
    for(int i = 0; i < casingVector.size(); i++)
    {
        //Update the wire to let it know about the possible interaction.
        auto &state = m_backend->state();
        state.casingHasStopLights(casingVector[i]);

        //Update the idea to let it show the correct stoplight.
        //The weirdness of this! A simulated mouse press and a simulated
        //hover event!
        WireCasingInteraction interaction(*casingVector[i],*m_backend);
        if(interaction.tryHover())
            possibleConnection = true;
    }
    showConnectionCursor(possibleConnection);

    event->accept();
}

void Wire::mouseReleaseEvent(QMouseEvent *event)
{   
    //Update information for this class.
    setDragging(false);
    ungrabMouse();
    m_backend->state().resetStopLights();

    //Event things
    event->accept();
    m_corkboard->setEnabled(true);

    //Reset cursor;
    QGuiApplication::restoreOverrideCursor();

    //Identify the search position in corkboard coordinates.
    QPointF totalDisplacement = m_initialDisplacement + m_currentScreenPosition - m_initialScreenPosition;
    QPointF scaledDisplacement = totalDisplacement / corkboard()->scale();
    QPointF searchPoint = m_initialPlugPosition + scaledDisplacement;

    //Search for a possible casing.
    auto casingsVector = m_corkboard->locateCasingNear(searchPoint);

    //Try the nearby casings.
    for(int i = 0; i < casingsVector.size(); i++)
    {
        WireCasingInteraction interaction(*casingsVector[i],*m_backend);
        if(interaction.tryConnect())
        {
            move();

            QJsonObject obj = m_backend->serialize();
            m_corkboard->createOrShiftWire(obj);
            return;
        }
    }

    //If the wire isn't valid, delete it.
    if(m_backend->state().requiresPlug())
    {
        if(!m_corkboard->destroyWire())
            m_corkboard->backend()->removeWire(m_backend);
        return;
    }
}

Corkboard *Wire::corkboard() const
{
    return m_corkboard;
}

WireBackend* Wire::backend() const
{
    return m_backend;
}

void Wire::setCorkboard(Corkboard *corkboard)
{
    m_corkboard = corkboard;
}

void Wire::setBackend(WireBackend *backend)
{
    m_backend = backend;
}

bool Wire::selected() const
{
    return m_selected;
}

void Wire::setSelected(bool selected)
{
    if (m_selected == selected)
        return;

    update();
    m_selected = selected;
    emit selectedChanged(m_selected);
}

void Wire::setInPoint(QPointF inPoint)
{
    if (m_inPoint == inPoint)
        return;

    m_inPoint = inPoint;
    emit inPointChanged(m_inPoint);
}

void Wire::setOutPoint(QPointF outPoint)
{
    if (m_outPoint == outPoint)
        return;

    m_outPoint = outPoint;
    emit outPointChanged(m_outPoint);
}

void Wire::setControlPoint1(QPointF controlPoint1)
{
    if (m_controlPoint1 == controlPoint1)
        return;

    m_controlPoint1 = controlPoint1;
    emit controlPoint1Changed(m_controlPoint1);
}

void Wire::setControlPoint2(QPointF controlPoint2)
{
    if (m_controlPoint2 == controlPoint2)
        return;

    m_controlPoint2 = controlPoint2;
    emit controlPoint2Changed(m_controlPoint2);
}

void Wire::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(m_color);
}

void Wire::setTransmitting(bool transmitting)
{
    if (m_transmitting == transmitting)
        return;

    if(transmitting == true)
    {
        startTimer(1000);
        m_transmitting = transmitting;
        emit transmittingChanged(m_transmitting);
    }
    else
    {
        m_transmitting = false;
        emit transmittingChanged(m_transmitting);
    }
}

void Wire::endTransmitting()
{
    setTransmitting(false);
}

void Wire::showConnectionCursor(bool validConnection)
{
    if(validConnection == m_validConnection)
        return;

    m_validConnection = validConnection;

    QGuiApplication::restoreOverrideCursor();

    QCursor cursor;
    if(m_validConnection)
    {
        cursor = QCursor(QPixmap(":/Images/ConnectWire.png"),2,2);
        QGuiApplication::setOverrideCursor(cursor);
    }
    else
    {
        cursor = QCursor(QPixmap(":/Images/StylizedCursor2.png"),2,2);
        QGuiApplication::setOverrideCursor(cursor);
    }
}

QColor Wire::color() const
{
    return m_color;
}

bool Wire::transmitting() const
{
    return m_transmitting;
}

void Wire::setDragging(bool dragging)
{
    if (m_dragging == dragging)
        return;

    m_dragging = dragging;
    emit draggingChanged(m_dragging);
}

bool Wire::dragging() const
{
    return m_dragging;
}

QPointF Wire::controlPoint1() const
{
    return m_controlPoint1;
}

QPointF Wire::controlPoint2() const
{
    return m_controlPoint2;
}

void Wire::updateControlPoints()
{
    std::pair<QPointF, QPointF> controlPoints = pointsC1C2();
    setControlPoint1(controlPoints.first);
    setControlPoint2(controlPoints.second);
}

QPointF Wire::inPoint() const
{
    return m_inPoint;
}

QPointF Wire::outPoint() const
{
    return m_outPoint;
}

const QPointF &Wire::getEndPoint(PlugType plugType) const
{
    Q_ASSERT(plugType != PlugType::None);

    return (plugType == PlugType::DataOut ? m_outPoint : m_inPoint);
}

void Wire::setEndPoint(PlugType plugType, const QPointF &point)
{
    switch (plugType)
    {
    case PlugType::DataOut:
        m_outPoint = point;
        break;

    case PlugType::DataIn:
        m_inPoint = point;
        break;

    default:
        break;
    }
}

void Wire::moveEndPoint(PlugType plugType, const QPointF &offset)
{
    switch (plugType)
    {
    case PlugType::DataOut:
        m_outPoint += offset;
        break;

    case PlugType::DataIn:
        m_inPoint += offset;
        break;

    default:
        break;
    }
}

void Wire::setBoundingRect(QPointF initialPos, QPointF displacement)
{
    QRectF basicRect = QRectF(initialPos,initialPos + displacement).normalized();
    std::pair<QPointF, QPointF> points = pointsC1C2(initialPos + displacement, initialPos);
    QRectF c1c2Rect = QRectF(points.first, points.second).normalized();

    double const diam = 12;
    QRectF commonRect = basicRect.united(c1c2Rect);
    QPointF const cornerOffset(diam, diam);

    commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
    commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);

    setX(commonRect.x());
    setY(commonRect.y());
    setWidth(commonRect.width());
    setHeight(commonRect.height());
}

std::pair<QPointF, QPointF> Wire::pointsC1C2() const
{
    const double defaultOffset = 200;
    double xDistance = m_inPoint.x() - m_outPoint.x();
    double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));
    double verticalOffset = 0;
    double ratioX = 0.5;

    if (xDistance <= 0)
    {
        double yDistance = m_inPoint.y() - m_outPoint.y() + 20;
        double vector = yDistance < 0 ? -1.0 : 1.0;
        verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;
        ratioX = 1.0;
    }

    horizontalOffset *= ratioX;

    QPointF c1(m_outPoint.x() + horizontalOffset, m_outPoint.y() + verticalOffset);
    QPointF c2(m_inPoint.x() - horizontalOffset, m_inPoint.y() - verticalOffset);
    return std::make_pair(c1, c2);
}

std::pair<QPointF, QPointF> Wire::pointsC1C2(QPointF inPoint, QPointF outPoint)
{
    const double defaultOffset = 200;
    double xDistance = inPoint.x() - outPoint.x();

    double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));
    double verticalOffset = 0;
    double ratioX = 0.5;

    if (xDistance <= 0)
    {
        double yDistance = inPoint.y() - outPoint.y() + 20;
        double vector = yDistance < 0 ? -1.0 : 1.0;
        verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;
        ratioX = 1.0;
    }

    horizontalOffset *= ratioX;

    QPointF c1(outPoint.x() + horizontalOffset, outPoint.y() + verticalOffset);
    QPointF c2(inPoint.x() - horizontalOffset, inPoint.y() - verticalOffset);
    return std::make_pair(c1, c2);
}


void Wire::timerEvent(QTimerEvent *event)
{
    m_transmitting = false;
    emit transmittingChanged(m_transmitting);
    killTimer(event->timerId());
}
