#ifndef WIRE_H
#define WIRE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QTimer>

#include "Plug.h"

class Corkboard;
class CorkboardBackend;
class WireBackend;

class Wire : public QQuickItem
{
    Q_OBJECT

    // State properties
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(bool dragging READ dragging WRITE setDragging NOTIFY draggingChanged)
    Q_PROPERTY(bool transmitting READ transmitting WRITE setTransmitting NOTIFY transmittingChanged)

    // Geometry Properties
    Q_PROPERTY(QPointF inPoint READ inPoint WRITE setInPoint NOTIFY inPointChanged)
    Q_PROPERTY(QPointF outPoint READ outPoint WRITE setOutPoint NOTIFY outPointChanged)
    Q_PROPERTY(QPointF controlPoint1 READ controlPoint1 WRITE setControlPoint1 NOTIFY controlPoint1Changed)
    Q_PROPERTY(QPointF controlPoint2 READ controlPoint2 WRITE setControlPoint2 NOTIFY controlPoint2Changed)

    // Styling properties.
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit Wire(QQuickItem *parent = nullptr);
    ~Wire() override;

    void finishSetup(QColor color);

    Corkboard* corkboard() const;
    WireBackend* backend() const;

    void setCorkboard(Corkboard* corkboard);
    void setBackend(WireBackend* backend);

    void simulateMousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    /// Call this method when a moving frame needs to drag along
    /// a wire.
    void move();

    bool selected() const;
    bool dragging() const;
    bool transmitting() const;

    QPointF inPoint() const;
    QPointF outPoint() const;
    QPointF controlPoint1() const;
    QPointF controlPoint2() const;

    QColor color() const;

    //Helper utilities.
    void updateControlPoints();

    QPointF const& getEndPoint(PlugType plugType) const;
    void setEndPoint(PlugType plugType, QPointF const& point);
    void moveEndPoint(PlugType plugType, QPointF const &offset);

    void setBoundingRect(QPointF initialPos, QPointF displacement);

    std::pair<QPointF, QPointF> pointsC1C2() const;
    static std::pair<QPointF, QPointF> pointsC1C2(QPointF inPoint, QPointF outPoint);

public slots:

    void setSelected(bool selected);
    void setDragging(bool dragging);

    void setInPoint(QPointF inPoint);
    void setOutPoint(QPointF outPoint);
    void setControlPoint1(QPointF controlPoint1);
    void setControlPoint2(QPointF controlPoint2);

    void setColor(QColor color);
    void setTransmitting(bool transmitting);
    void endTransmitting();

signals:
    void selectedChanged(bool selected);
    void draggingChanged(bool dragging);
    void inPointChanged(QPointF inPoint);
    void outPointChanged(QPointF outPoint);
    void controlPoint1Changed(QPointF controlPoint1);
    void controlPoint2Changed(QPointF controlPoint2);
    void colorChanged(QColor color);

    void transmittingChanged(bool transmitting);

private:
    Corkboard* m_corkboard;
    WireBackend* m_backend;

    //Methods when a wire is moved/moving.
    QPointF m_clickPos;
    QPointF m_lastClickPos;

    QPointF m_initialPlugPosition;
    QPointF m_initialScreenPosition;
    QPointF m_currentScreenPosition;
    QPointF m_initialDisplacement;

    bool m_dragging;
    bool m_selected;
    bool m_transmitting;

    //The actual geometry.
    QPointF m_inPoint;
    QPointF m_outPoint;
    QPointF m_controlPoint1;
    QPointF m_controlPoint2;

    QColor m_color;

    //Make the cursor show what's going to happen
    void showConnectionCursor(bool validConnection);
    bool m_validConnection;
};

#endif // WIRE_H
