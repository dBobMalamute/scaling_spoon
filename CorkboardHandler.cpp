#include "CorkboardHandler.h"
#include <QDir>
#include <QFileDialog>
#include <QThread>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include <QQmlApplicationEngine>

#include "Utilities/UtilitiesPlugin.h"
#include "Calculator/CalculatorPlugin.h"
#include "Distributions/DistributionsPlugin.h"
#include "Numbers/NumbersPlugin.h"

#include "Registries/DataTypeRegistry.h"
#include "Registries/IdeaRegistry.h"
#include "Registries/ToolbarRegistry.h"

void CorkboardHandler::LoadPlugins()
{
    loadPlugin(new DistributionsPlugin());
    loadPlugin(new UtilitiesPlugin());
    loadPlugin(new NumbersPlugin());
    loadPlugin(new CalculatorPlugin());
}

CorkboardHandler::CorkboardHandler(QObject *parent) :
    QObject(parent),
    openFileUrl("")
{
    //Initialize the registries
    m_ideaRegistry = std::make_shared<IdeaRegistry>();
    m_dataTypeRegistry = std::make_shared<DataTypeRegistry>();
    m_toolbarRegistry = std::make_shared<ToolbarRegistry>();

    LoadPlugins();

    m_ideaList = m_ideaRegistry->registeredIdeaNames();
    m_collectionList = m_toolbarRegistry->toolbarList();

    for(const QString &toolbar : m_toolbarRegistry->toolbarList())
    {
        m_ideasInCollectionMap[toolbar] = m_toolbarRegistry->toolbarIdeas(toolbar);
    }

    emit collectionListChanged();

    QSettings settings;
    m_recentFiles = settings.value("recentFiles").toStringList();
    emit recentFilesChanged();
}

void CorkboardHandler::loadInitialFile()
{
    load(QUrl("qrc:/Muts/InitialFile.mutt"));
}

CorkboardHandler::~CorkboardHandler()
{
    QSettings settings;
    settings.setValue("recentFiles", m_recentFiles);
}

Corkboard *CorkboardHandler::getCorkboard() const
{
    return m_corkboard;
}

void CorkboardHandler::resetCorkboard(Corkboard *corkboard)
{
    m_corkboard = corkboard;

    m_corkboard->createBackend(m_ideaRegistry);
    m_corkboard->setQmlEngine(qmlEngine(corkboard));
    m_corkboard->setDataTypeRegistry(m_dataTypeRegistry);

    //loadInitialFile();
}

bool CorkboardHandler::isModified()
{
    //If you can undo something, it's been modified. Right? I really hope this works...
    return m_corkboard->canUndo();
}

void CorkboardHandler::setVisibleWindowProperties(int width, int height)
{
    m_corkboard->setVisibleWindowParams(width,height);
}

void CorkboardHandler::loadPlugin(MalamutePlugin* plugin)
{
    if(plugin)
    {
        //DO NOT CHANGE THE ORDER. Datatypes, ideas, toolbars.
        plugin->registerDataTypeAttributes(m_dataTypeRegistry);
        plugin->registerIdeas(m_ideaRegistry);
        plugin->registerToolbars(m_toolbarRegistry);
    }
    delete plugin;
}

QStringList CorkboardHandler::collectionList() const
{
    return m_collectionList;
}

QStringList CorkboardHandler::paletteList() const
{
    return m_paletteList;
}

void CorkboardHandler::setPalleteList(QString collectionName)
{
    m_paletteList.clear();
    m_paletteList = m_toolbarRegistry->toolbarIdeas(collectionName);

    emit paletteListChanged();
}

QStringList CorkboardHandler::collectionIdeaList(QString collectionName) const
{
    return m_ideasInCollectionMap[collectionName];
}

QString CorkboardHandler::ideaIconPath(QString ideaName)
{
    QString iconPath = m_ideaRegistry->iconPath(ideaName);
    if(iconPath == "")
        return "qrc:/Images/Blank.png";
    else
        return iconPath;
}

QStringList CorkboardHandler::ideaList() const
{
    return m_ideaList;
}

void CorkboardHandler::save()
{
    QString filePath = openFileUrl.toLocalFile();
    if(QFileInfo(filePath).suffix() != "mutt")
    {
        filePath.append(".mutt");
    }
    QFile file(filePath);

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(m_corkboard->saveData());
    file.close();
}

void CorkboardHandler::saveAs(const QUrl &fileUrl)
{
    openFileUrl = fileUrl;
    adjustRecentFilesList();
    QString filePath = fileUrl.toLocalFile();
    if(QFileInfo(filePath).suffix() != "mutt")
    {
        filePath.append(".mutt");
    }
    QFile file(filePath);

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(m_corkboard->saveData());
    file.close();

    setOpenFileUrl(fileUrl);
}

void CorkboardHandler::load(const QUrl &fileUrl)
{
    m_corkboard->deleteAll();

    QString filePath = fileUrl.toDisplayString();
    if(filePath.startsWith("file"))
        filePath.remove(0,7);
    if(filePath.startsWith("qrc"))
        filePath.remove(0,3);

    if (!QFileInfo::exists(filePath))
        return;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
        return;

    m_corkboard->loadData(file.readAll());

    file.close();

    setOpenFileUrl(fileUrl);
    adjustRecentFilesList();
}

void CorkboardHandler::revert()
{
    load(openFileUrl);
}

void CorkboardHandler::adjustRecentFilesList()
{
    m_recentFiles.removeAll(openFileUrl.toString());
    m_recentFiles.prepend(openFileUrl.toString());

    while(m_recentFiles.size() > 10)
        m_recentFiles.removeLast();

    emit recentFilesChanged();
}

QUrl CorkboardHandler::getOpenFileUrl() const
{
    return openFileUrl;
}

bool CorkboardHandler::validOpenFileUrl() const
{
    return openFileUrl.isEmpty() ? false : true;
}

void CorkboardHandler::setOpenFileUrl(const QUrl &value)
{
    openFileUrl = value;
}

QStringList CorkboardHandler::recentFiles() const
{
    return m_recentFiles;
}

void CorkboardHandler::clearRecentFiles()
{
    m_recentFiles.clear();
    emit recentFilesChanged();
}
