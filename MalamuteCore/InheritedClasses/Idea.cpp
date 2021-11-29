#include "Idea.h"

#include <QClipboard>
#include <QMimeData>
#include <QApplication>

Idea::~Idea()
{

}

QJsonObject Idea::save() const
{
    return QJsonObject();
}

void Idea::load(const QJsonObject &)
{

}

void Idea::onNewDataIn(std::shared_ptr<DataType>, PlugNumber)
{

}

std::shared_ptr<DataType> Idea::dataOut(PlugNumber)
{
    return nullptr;
}

void Idea::resetDisplay()
{

}

void Idea::finishSetupQmlContext()
{

}

QString Idea::shortStatusMessage() const
{
    return m_shortStatusMessage;
}

QString Idea::longStatusMessage() const
{
    return m_longStatusMessage;
}

IdeaStatus Idea::status() const
{
    return m_status;
}

bool Idea::valid() const
{
    if(m_status == IdeaStatus::Working || m_status == IdeaStatus::Warning || m_status == IdeaStatus::DisplayError)
        return true;
    else
        return false;
}

void Idea::setStatus(IdeaStatus status, QString shortMessage, QString longMessage)
{
    m_status = status;
    m_shortStatusMessage = shortMessage;
    m_longStatusMessage = longMessage;
    emit statusChanged();

    if(status == IdeaStatus::StandBy || status == IdeaStatus::Error || status == IdeaStatus::Fatal)
    {
        emit clearDataAllPlugs();
        emit validChanged(false);
    }
    else
    {
        emit validChanged(true);
    }
}

void Idea::setDefaultStatus()
{
    IdeaStatus oldStatus = m_status;
    m_status = IdeaStatus::StandBy;
    m_shortStatusMessage = "Standing By";
    m_longStatusMessage = "Waiting for user input";
    emit statusChanged();
    emit validChanged(false);

    //Clear data if necessary
    if(oldStatus == IdeaStatus::Working || oldStatus == IdeaStatus::Warning || oldStatus == IdeaStatus::DisplayError)
        emit clearDataAllPlugs();
}

void Idea::sendTextToClipboard(QString text)
{
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(text);

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}
