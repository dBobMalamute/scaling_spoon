import QtQuick 2.12
import QtQuick.Controls 2.12

MenuBar
{
    z: 100
    //    Menu
    //    {
    //        id: fileMenu
    //        title: "File"
    //        width: 200
    //        MenuItem { action: newAction}
    //        MenuItem { action: openAction}
    //        Menu
    //        {
    //            id: recentFilesSubMenu
    //            title: "Recent Files: " + recentFilesInstantiator.count
    //            enabled: recentFilesInstantiator.count > 0

    //            Instantiator
    //            {
    //                id: recentFilesInstantiator
    //                model: malamuteHandler.recentFiles;
    //                delegate: MenuItem
    //                {
    //                    text: modelData.split("\\").pop().split("/").pop()
    //                    onTriggered:
    //                    {
    //                        malamuteHandler.load(modelData)
    //                    }
    //                }
    //                onObjectAdded: recentFilesSubMenu.insertItem(index, object)
    //                onObjectRemoved: recentFilesSubMenu.removeItem(object)
    //            }

    //            MenuSeparator {}

    //            MenuItem {
    //                text: qsTr("Clear Menu")
    //                onTriggered: malamuteHandler.clearRecentFiles();
    //            }
    //        }
    //        MenuSeparator {}
    //        MenuItem { action: saveAction}
    //        MenuItem { action: saveAsAction}
    //        MenuSeparator {}
    //        MenuItem { action: exitAction}
    //    }

    Menu
    {
        title: "Edit"
        MenuItem{ action: undoAction}
        MenuItem{ action: redoAction}
        MenuSeparator{}
        MenuItem{ action: cutAction}
        MenuItem{ action: copyAction}
        MenuItem{ action: pasteAction}
        MenuItem{ action: duplicateAction}
        MenuSeparator{}
        MenuItem{ action: deleteAction}
        MenuItem{ action: deleteAll}
        MenuSeparator{}
        MenuItem{ action: selectAll}
        MenuItem{ action: deselectAll}

    }

    Menu
    {
        title: "View"
        MenuItem {action: defaultView}
        MenuSeparator{}
        MenuItem {action: centerView}
        MenuItem {action: zoomIn}
        MenuItem {action: zoomOut}
        MenuItem {action: frameLessWindow}
    }

    Menu
    {
        id: ideaMenu
        title: "Ideas"

        Instantiator
        {
            id: ideaCollectionList
            model: malamuteHandler.collectionList;
            delegate: Menu
            {
                id: tempId
                width: 300
                title: modelData
                Instantiator
                {
                    model: malamuteHandler.collectionIdeaList(tempId.title)
                    delegate: MenuItem
                    {
                        text: modelData;
                        icon.source: malamuteHandler.ideaIconPath(modelData)
                        icon.color: "transparent"
                        onTriggered:
                        {
                            corkboard.createIdea(modelData,Qt.point(applicationWindow.width/3,applicationWindow.height/3))
                        }
                    }
                    onObjectAdded: tempId.insertItem(index, object)
                }
            }
            onObjectAdded: ideaMenu.insertMenu(index+1, object)
        }
    }

    Menu
    {
        title: "Help"
        MenuItem {action: aboutAction}
        MenuItem {action: licenseAction}
    }

}
