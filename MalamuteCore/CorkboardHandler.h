#ifndef CORKBOARDHANDLER_H
#define CORKBOARDHANDLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>

#include "Corkboard.h"
#include "Casing.h"
#include "Wire.h"

#include "InheritedClasses/MalamutePlugin.h"

class CorkboardHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList recentFiles READ recentFiles NOTIFY recentFilesChanged)
    Q_PROPERTY(QStringList collectionList READ collectionList NOTIFY collectionListChanged)
    Q_PROPERTY(QStringList paletteList READ paletteList NOTIFY paletteListChanged)
    Q_PROPERTY(QStringList ideaList READ ideaList NOTIFY ideaListChanged)

public:
    explicit CorkboardHandler(QObject *parent = nullptr);
    ~CorkboardHandler() override;

    Corkboard *getCorkboard() const;

    ///Call this method after you've loaded new idea collections to add them to corkboard.
    Q_INVOKABLE void resetCorkboard(Corkboard *corkboard);

    Q_INVOKABLE bool isModified();
    Q_INVOKABLE void save();
    Q_INVOKABLE void saveAs(const QUrl &fileUrl);
    Q_INVOKABLE void load(const QUrl &fileUrl);
    void loadInitialFile();
    Q_INVOKABLE void revert();

    Q_INVOKABLE void adjustRecentFilesList();

    Q_INVOKABLE QUrl getOpenFileUrl() const;
    Q_INVOKABLE bool validOpenFileUrl() const;
    void setOpenFileUrl(const QUrl &value);

    QStringList recentFiles() const;
    Q_INVOKABLE void clearRecentFiles();

    Q_INVOKABLE void setVisibleWindowProperties(int width, int height);

    QStringList registeredIdeas() const;
    void loadPlugin(MalamutePlugin *plugin);

    QStringList collectionList() const;
    QStringList paletteList() const;
    Q_INVOKABLE void setPalleteList(QString collectionName);
    Q_INVOKABLE QStringList collectionIdeaList(QString collectionName) const;
    Q_INVOKABLE QString ideaIconPath(QString ideaName);

    QStringList ideaList() const;

signals:
    void recentFilesChanged();
    void collectionListChanged();
    void paletteListChanged();
    void ideaListChanged();

private:
    Corkboard* m_corkboard;

    QUrl openFileUrl;
    QStringList m_recentFiles;

    std::shared_ptr<IdeaRegistry> m_ideaRegistry;
    std::shared_ptr<DataTypeRegistry> m_dataTypeRegistry;
    std::shared_ptr<ToolbarRegistry> m_toolbarRegistry;

    QDir pluginsDir;
    QStringList pluginFileNames;

    QStringList m_collectionList;
    QStringList m_paletteList;
    QMap<QString, QStringList> m_ideasInCollectionMap;
    QStringList m_ideaList;
    void LoadPlugins();
};

#endif // CORKBOARDHANDLER_H
