#include "PostIt.h"
#include "Corkboard.h"

PostIt::PostIt(QQuickItem *parent) : QQuickItem(parent),
    m_selected(true),
    m_moving(false),
    m_title(""),
    m_shortMessage(""),
    m_longMessage("")
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAntialiasing(true);
    setZ(1);
}

PostIt::~PostIt()
{

}

QJsonObject PostIt::save() const
{
    QJsonObject saveJson;
    saveJson["id"] = m_uid.toString();
    saveJson["x"] = x();
    saveJson["y"] = y();
    saveJson["title"] = m_title;
    saveJson["smsg"] = m_shortMessage;
    saveJson["lmsg"] = m_longMessage;
    return saveJson;
}

void PostIt::load(const QJsonObject &p)
{
    QJsonValue v = p["id"];
    if(!v.isUndefined())
        m_uid = QUuid(v.toString());

    v = p["x"];
    if(!v.isUndefined())
        setX(v.toDouble());

    v = p["y"];
    if(!v.isUndefined())
        setY(v.toDouble());

    v = p["title"];
    if(!v.isUndefined())
        setTitle(v.toString());

    v = p["smsg"];
    if(!v.isUndefined())
        setShortMessage(v.toString());

    v = p["lmsg"];
    if(!v.isUndefined())
        setLongMessage(v.toString());
}

void PostIt::mousePressEvent(QMouseEvent *event)
{
    setFocus(true);

    //Selection, shift, etc.
    if(!(event->modifiers() & Qt::ShiftModifier))
    {
        if(selected() == false)
            m_corkboard->deselectAll();
    }
    setSelected(true);

    initialClickPos = event->globalPos();
    initialXYPos = position();
    m_moving = true;
    m_corkboard->prepareMoveSelection();
    event->accept();
}

void PostIt::mouseMoveEvent(QMouseEvent *event)
{
    // requiring left button allows for right mouse button to be clicked to end a drag. I don't know why I did
    // this, but I like it. - dBob
    if(m_moving && event->buttons() & Qt::LeftButton)
    {
        QPointF displacement = event->globalPos() - initialClickPos;
        m_corkboard->moveSelection(displacement);
    }
    event->accept();
}

void PostIt::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_moving)
    {
        m_corkboard->finishMoveSelection();
        m_moving = false;
    }
    event->accept();
}

bool PostIt::selected() const
{
    return m_selected;
}

QString PostIt::title() const
{
    return m_title;
}

QString PostIt::shortMessage() const
{
    return m_shortMessage;
}

QString PostIt::longMessage() const
{
    return m_longMessage;
}

void PostIt::setSelected(bool selected)
{
    if (m_selected == selected)
        return;

    m_selected = selected;
    emit selectedChanged(m_selected);
}

void PostIt::setTitle(QString title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged(m_title);
}

void PostIt::setShortMessage(QString shortMessage)
{
    if (m_shortMessage == shortMessage)
        return;

    m_shortMessage = shortMessage;
    emit shortMessageChanged(m_shortMessage);
}

void PostIt::setLongMessage(QString longMessage)
{
    if (m_longMessage == longMessage)
        return;

    m_longMessage = longMessage;
    emit longMessageChanged(m_longMessage);
}

void PostIt::deletePostIt()
{
    m_corkboard->removePostIt(this->save());
}

void PostIt::setCorkboard(Corkboard *corkboard)
{
    m_corkboard = corkboard;
}

QUuid PostIt::id() const
{
    return m_uid;
}
