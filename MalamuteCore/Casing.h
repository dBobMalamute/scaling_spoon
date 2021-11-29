//This class will (HAS) eventually overtake IdeaWrapperGraphicsObject
// Will replace that thing. :)

#ifndef CASING_H
#define CASING_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QQmlListProperty>
#include <QTimer>

#include "WireBackend.h"
#include "CasingState.h"

class Corkboard;

class Casing : public QQuickItem
{
    Q_OBJECT

    //Geometry, color
    Q_PROPERTY(QSize casingSize READ casingSize WRITE setCasingSize NOTIFY casingSizeChanged)
    Q_PROPERTY(int predefinedColor READ predefinedColor WRITE setPredefinedColor NOTIFY predefinedColorChanged)

    //Name. If shown.
    Q_PROPERTY(QString ideaName READ ideaName NOTIFY ideaNameChanged)
    Q_PROPERTY(QSize nameSize READ nameSize NOTIFY nameSizeChanged)

    // Properties required for the plugs.
    Q_PROPERTY(int plugBoxTop READ plugBoxTop WRITE setPlugBoxTop NOTIFY plugBoxTopChanged)
    Q_PROPERTY(int inLabelWidth READ inLabelWidth WRITE setInLabelWidth NOTIFY inLabelWidthChanged)
    Q_PROPERTY(int outLabelWidth READ outLabelWidth WRITE setOutLabelWidth NOTIFY outLabelWidthChanged)
    Q_PROPERTY(QStringList inPlugLabels READ inPlugLabels NOTIFY inPlugLabelsChanged)
    Q_PROPERTY(QStringList outPlugLabels READ outPlugLabels NOTIFY outPlugLabelsChanged)

    // Properties for the validation box. Also called the feedback box?
    Q_PROPERTY(QColor validationColor READ validationColor WRITE setValidationColor NOTIFY validationColorChanged)
    Q_PROPERTY(QString validationMessage READ validationMessage WRITE setValidationMessage NOTIFY validationMessageChanged)
    Q_PROPERTY(int validationBoxTop READ validationBoxTop WRITE setValidationBoxTop NOTIFY validationBoxTopChanged)
    Q_PROPERTY(QSize validationBoxSize READ validationBoxSize WRITE setValidationBoxSize NOTIFY validationBoxSizeChanged)

    // Properties which are shown to the user.
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(bool resizeable READ resizeable NOTIFY resizeableChanged)

    // Multi-document interface type things
    Q_PROPERTY(bool minimizable READ minimizable NOTIFY minimizableChanged)
    Q_PROPERTY(bool minimized READ minimized WRITE setMinimized NOTIFY minimizedChanged)
    Q_PROPERTY(bool hasExtraQML READ hasExtraQML WRITE setHasExtraQML NOTIFY hasExtraQMLChanged)

public:
    explicit Casing(QQuickItem *parent = nullptr);
    ~Casing() override;

    void finishSetup();
    void setupCaption();
    void setupInPlugs();
    void setupOutPlugs();
    void setupValidationBox();

    QJsonObject save() const;
    void load (QJsonObject const &json);

    Corkboard* corkboard() const;
    CasingBackend* casingBackend();

    void setCorkboard(Corkboard* corkboard);
    void setBackend(CasingBackend* backend);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;

    /// Recalculates the size of the box. Called when things change.
    void recalculateSize();
    QPointF plugScenePosition(PlugNumber index, PlugType plugType) const;
    PlugNumber checkHitScenePoint(PlugType plugType, QPointF point) const;
    PlugNumber checkHitScenePointHighTolerance(PlugType plugType, QPointF point) const;
    bool checkHitResizer(QPointF point) const;
    void repositionQML() const;

    // Methods to allow qml to get color data.
    Q_INVOKABLE QColor inPlugColor(int plug);
    Q_INVOKABLE QColor outPlugColor(int plug);

public:
    QSize casingSize() const;

    int predefinedColor() const;

    QString ideaName() const;
    QSize nameSize() const;

    int plugBoxTop() const;
    int inLabelWidth() const;
    int outLabelWidth() const;
    QStringList inPlugLabels() const;
    QStringList outPlugLabels() const;

    QColor validationColor() const;
    QString validationMessage() const;
    int validationBoxTop() const;
    QSize validationBoxSize() const;

    bool embeddedQML() const;

    bool selected() const;
    bool resizeable() const;

    bool minimizable() const;
    bool minimized() const;
    bool hasExtraQML() const;

signals:
    void casingSizeChanged(QSize casingSize);

    void predefinedColorChanged(int predefinedColor);

    void ideaNameChanged(QString ideaName);
    void nameSizeChanged(QSize nameSize);

    void plugBoxTopChanged(int plugBoxTop);
    void inLabelWidthChanged(int inLabelWidth);
    void outLabelWidthChanged(int outLabelWidth);
    void inPlugLabelsChanged(QStringList inPlugLabels);
    void outPlugLabelsChanged(QStringList outPlugLabels);

    void validationColorChanged(QColor validationColor);
    void validationMessageChanged(QString validationMessage);
    void validationBoxTopChanged(int validationBoxTop);
    void validationBoxSizeChanged(QSize validationBoxSize);

    void selectedChanged(bool selected);
    void resizeableChanged(bool resizeable);

    void minimizableChanged(bool minimizable);
    void minimizedChanged(bool minimized);
    void hasExtraQMLChanged(bool hasExtraQML);


public slots:
    void setCasingSize(QSize casingSize);

    void setPredefinedColor(int predefinedColor);

    void setPlugBoxTop(int plugBoxTop);
    void setInLabelWidth(int inLabelWidth);
    void setOutLabelWidth(int outLabelWidth);

    void setValidationColor(QColor validationColor);
    void setValidationMessage(QString validationMessage);
    void setValidationBoxTop(int validationBoxTop);
    void setValidationBoxSize(QSize validationBoxSize);

    void setSelected(bool selected);
    void minimizeButtonClicked();
    void setMinimized(bool minimized);
    void extraButtonClicked();
    void setHasExtraQML(bool hasExtraQML);

private:
    void hoverEvents(QHoverEvent *event);

    Corkboard* m_corkboard;
    CasingBackend* m_backend;

    QPointF initialClickPos;
    QPointF initialSize;

    bool m_moving;
    bool m_resizing;

    int m_outPlugs;
    int m_inPlugs;

    bool m_hasEmbeddedQML;
    bool m_hasMinimizedQML;

    int m_QMLBoxX;
    int m_QMLBoxY;
    int m_QMLBoxWidth;
    int m_QMLBoxHeight;

private:
    QSize m_casingSize;

    int m_predefinedColor;

    QString m_ideaName;
    QSize m_nameSize;

    int m_plugBoxTop;
    int m_inLabelWidth;
    int m_outLabelWidth;
    QStringList m_inPlugLabels;
    QStringList m_outPlugLabels;
    QList<QColor> m_inPlugColors;
    QList<QColor> m_outPlugColors;

    QColor m_validationColor;
    QString m_validationMessage;
    int m_validationBoxTop;
    QSize m_validationBoxSize;

    bool m_selected;
    bool m_resizeable;

    bool m_minimizable;
    bool m_minimized;
    bool m_minimizedSetDuringLoad; // This handles an edge case I did not see coming.
    bool m_changeNameOnMinimized;
    bool m_hasExtraQML;
};

#endif // CASING_H
