#ifndef POSTIT_H
#define POSTIT_H

#include <QQuickItem>
#include <QUuid>

class Corkboard;

class PostIt : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString shortMessage READ shortMessage WRITE setShortMessage NOTIFY shortMessageChanged)
    Q_PROPERTY(QString longMessage READ longMessage WRITE setLongMessage NOTIFY longMessageChanged)

public:
    explicit PostIt(QQuickItem* parent = nullptr);
    ~PostIt() override;

    QJsonObject save() const;
    void load(QJsonObject const &p);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    bool selected() const;
    QString title() const;
    QString shortMessage() const;
    QString longMessage() const;

signals:
    void selectedChanged(bool selected);
    void titleChanged(QString title);
    void shortMessageChanged(QString shortMessage);
    void longMessageChanged(QString longMessage);

public slots:
    void setSelected(bool selected);
    void setTitle(QString title);
    void setShortMessage(QString shortMessage);
    void setLongMessage(QString longMessage);
    void deletePostIt();

public:
    Corkboard* m_corkboard;
    void setCorkboard(Corkboard *corkboard);

    QUuid id() const;

private:
    QUuid m_uid;
    QPointF initialClickPos;
    QPointF initialXYPos;
    bool m_selected;
    bool m_moving;

    QString m_title;
    QString m_shortMessage;
    QString m_longMessage;
};

#endif // POSTIT_H
