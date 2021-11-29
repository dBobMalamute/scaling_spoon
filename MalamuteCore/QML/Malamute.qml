import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.1
import QtGraphicalEffects 1.0
import QtQml.Models 2.12

import "PopupMenuFunctions.js" as Popups

import com.malamute.core 1.0

ApplicationWindow
{
    id: applicationWindow
    visible: true
    width: 1000
    height: 700 + palleteBar.height + singleMalamuteMenus.height
    title: qsTr("Malamute")
    color: "black"

    onWidthChanged:
    {
        corkboard.setVisibleWindowParams(width,height - 107)
    }
    onHeightChanged:
    {
        corkboard.setVisibleWindowParams(width,height - 107)
    }

    MalamuteCorkboardHandler
    {
        id: malamuteHandler
        Component.onCompleted:
        {
            resetCorkboard(corkboard);
        }
    }

    Action
    {
        id: newAction
        text: "&New"
        icon.name: "document-new"
        shortcut: StandardKey.New
        onTriggered:
        {
            corkboard.deleteAll();
        }
    }

    Action
    {
        id: openAction
        text: "&Open"
        shortcut: StandardKey.Open
        icon.name: "document-open"
        onTriggered:
        {
            openFileDialog.open();
        }
    }

    Action
    {
        id: saveAction
        text: "&Save"
        shortcut: StandardKey.Save
        icon.name: "document-save"
        onTriggered:
        {
            if(malamuteHandler.validOpenFileUrl())
                malamuteHandler.save();
            else
                saveAsFileDialog.open();
        }
    }

    Action
    {
        id: saveAsAction
        text: "&SaveAs..."
        shortcut: StandardKey.SaveAs
        icon.name: "document-save"
        onTriggered:
        {
            saveAsFileDialog.open();
        }
    }

    Action
    {
        id: exitAction
        text: "E&xit"
        icon.name: "document-close"
        onTriggered:
        {
            Qt.quit();
        }
    }

    //End of actions for file menu, start actions edit menu
    Action
    {
        id: undoAction
        text: "Undo";
        icon.name: "edit-undo"
        onTriggered:
        {
            corkboard.undo();
        }
    }

    Action
    {
        id: redoAction
        text: "Redo";
        icon.name: "edit-redo"
        onTriggered:
        {
            corkboard.redo();
        }
    }

    Action
    {
        id: cutAction
        text: "Cut";
        icon.name: "edit-cut"
        onTriggered:
        {
            corkboard.cut();
        }
    }

    Action
    {
        id: copyAction
        text: "Copy";
        icon.name: "edit-copy"
        onTriggered:
        {
            corkboard.copy();
        }
    }

    Action
    {
        id: pasteAction
        text: "Paste";
        icon.name: "edit-paste"
        onTriggered:
        {
            corkboard.paste();
        }
    }

    Action
    {
        id: duplicateAction
        text: "Duplicate"
        icon.name: "edit-copy"
        onTriggered:
        {
            corkboard.duplicate();
        }
    }

    Action
    {
        id: deleteAction
        text: "Delete"
        icon.name: "edit-delete"
        onTriggered:
        {
            corkboard.deleteSelection();
        }
    }

    Action
    {
        id: deleteAll
        text: "Delete All"
        icon.name: "edit-delete"
        onTriggered:
        {
            corkboard.deleteAll();
        }
    }

    Action
    {
        id: selectAll
        text: "Select All"
        icon.name: "edit-selectall"
        onTriggered:
        {
            corkboard.selectAll();
        }
    }

    Action
    {
        id: deselectAll
        text: "Deselect All";
        icon.name: "edit-selectall"
        onTriggered:
        {
            corkboard.deselectAll();
        }
    }

    //View Actions.
    Action
    {
        id: defaultView
        text: "Default View"
        onTriggered:
        {
            corkboard.resetScale();
            corkboard.centerView();
        }
    }

    Action
    {
        id: centerView
        text: "Center View"
        onTriggered:
        {
            corkboard.centerView();
        }
    }

    Action
    {
        id: zoomIn
        text: "Zoom in"
        onTriggered:
        {
            corkboard.zoomIn();
        }
    }

    Action
    {
        id: zoomOut
        text: "Zoom Out"
        onTriggered:
        {
            corkboard.zoomOut();
        }
    }

    Action
    {
        id: frameLessWindow
        text: "Show Frame"

        property bool showFrame: true

        onTriggered:
        {
            if(showFrame)
            {
                showFrame = false;
                applicationWindow.flags = Qt.Window | Qt.FramelessWindowHint
            }
            else
            {
                showFrame = true;
                applicationWindow.flags = Qt.Window
            }
        }
    }

    Action
    {
        id: aboutAction
        text: "About"
        onTriggered:
        {
            Popups.createAboutMenu();
        }
    }

    Action
    {
        id: licenseAction
        text: "License"
        onTriggered:
        {
            Popups.createLegalMenu();
        }
    }



    Action
    {
        id: casingMenuShowDetails
        text: "Details"
        onTriggered:
        {
            corkboard.showDetailsFromCasingMenu();
        }
    }

    Action
    {
        id: casingMenuMaximize
        text: "Minimize";
        onTriggered:
        {
            corkboard.minimizeFromCasingMenu();
        }
    }

    Action
    {
        id: casingMenuShowControls
        text: "Show Controls";
        onTriggered:
        {
            corkboard.showControlsFromCasingMenu();
        }
    }

    Action
    {
        id: casingMenuCopyState
        text: "Copy State";
        onTriggered:
        {
            corkboard.copyStateFromCasingMenu();
        }
    }

    Action
    {
        id: casingMenuPasteState
        text: "Paste State";
        onTriggered:
        {
            corkboard.pasteStateFromCasingMenu();
        }
    }

    Action
    {
        id: casingMenuDelete
        text: "Delete"
        onTriggered:
        {
            corkboard.deleteFromCasingMenu();
        }
    }

    Action
    {
        id: casingMenuSetColor1
        text: "Green"
        onTriggered:
        {
            corkboard.setColorFromCasingMenu(0);
        }
    }
    Action
    {
        id: casingMenuSetColor2
        text: "Olive"
        onTriggered:
        {
            corkboard.setColorFromCasingMenu(1);
        }
    }
    Action
    {
        id: casingMenuSetColor3
        text: "Maroon"
        onTriggered:
        {
            corkboard.setColorFromCasingMenu(2);
        }
    }
    Action
    {
        id: casingMenuSetColor4
        text: "Teal"
        onTriggered:
        {
            corkboard.setColorFromCasingMenu(3);
        }
    }
    Action
    {
        id: casingMenuSetColor5
        text: "Purple"
        onTriggered:
        {
            corkboard.setColorFromCasingMenu(4);
        }
    }
    Action
    {
        id: casingMenuSetColor6
        text: "Brown"
        onTriggered:
        {
            corkboard.setColorFromCasingMenu(5);
        }
    }

    menuBar: SingleMalamuteMenus
    {
        id: singleMalamuteMenus
    }

    FileDialog
    {
        id: saveAsFileDialog
        title: "Save File As"
        nameFilters: ["Malamute files (*.mutt)"]
        selectExisting: false
        folder: shortcuts.documents
        onAccepted:
        {
            malamuteHandler.saveAs(saveAsFileDialog.fileUrl)
        }
    }
    FileDialog
    {
        id: openFileDialog
        title: "Load File"
        nameFilters: ["Malamute files (*.mutt)"]
        selectExisting: true
        folder: shortcuts.documents
        onAccepted:
        {
            malamuteHandler.load(openFileDialog.fileUrl);
        }
    }

    header: PaletteBar
    {
        id: palleteBar
    }

    //Provides tool tips for the palleteBar. Because of how i did the geometry it's at the same level.
    PaletteToolTip
    {
        id: paletteToolTip
    }

    MalamuteCorkboard
    {
        id: corkboard;
        width: 4000
        height: 4000
        x: -1500
        y: -1650

        Rectangle
        {
            //Selection Rectangle.
            id: selectionRectangle
            border.color: CorkboardStyle.selectionRectangleColor()
            border.width: CorkboardStyle.selectionRectangleWidth()
            color: "transparent"
            visible: corkboard.selecting
            radius: CorkboardStyle.selectionRectangleRadius()
            z: 1
            Connections
            {
                target: corkboard;
                function onSelectingBoxChanged(box)
                {
                    selectionRectangle.x = box.x;
                    selectionRectangle.y = box.y;
                    selectionRectangle.width = box.width;
                    selectionRectangle.height = box.height;
                }
            }

        }

        Image
        {
            //The corkboard image itself.
            id: corkboardImage
            anchors.fill: parent
            source: CorkboardStyle.backgroundImagePath();
            fillMode: Image.Tile
            z: -9
        }
        ColorOverlay
        {
            id: tintOverlay;
            anchors.fill: corkboardImage
            source: corkboardImage
            color: CorkboardStyle.backgroundTintColor();
        }
    }

    Menu
    {
        id: contextMenu

        MenuItem{action: undoAction}
        MenuItem{action: redoAction}
        MenuSeparator{}
        MenuItem{ action: cutAction}
        MenuItem{ action: copyAction}
        MenuItem{ action: pasteAction}
        MenuSeparator{}
        MenuItem{ action: duplicateAction}
        MenuItem{ action: deleteAction}

        Connections
        {
            target: corkboard
            function onContextMenuActiveChanged(active)
            {
                if(active)
                {
                    contextMenu.x = corkboard.contextMenuPt.x - 20;
                    contextMenu.y = corkboard.contextMenuPt.y - 180;
                    contextMenu.open();
                }
            }
        }
        onClosed:
        {
            corkboard.setContextMenuActive(false);
        }
    }

    Menu
    {
        id: casingMenu

        MenuItem{action: casingMenuShowDetails}
        MenuItem{action: casingMenuMaximize}
        MenuItem{action: casingMenuShowControls}
        MenuSeparator{}
        Menu
        {
            id: colorsSubMenu
            title: "Change Color"
            MenuItem{action: casingMenuSetColor1; icon.source: "/Images/CasingColors1.png"; icon.color: "transparent"}
            MenuItem{action: casingMenuSetColor2; icon.source: "/Images/CasingColors2.png"; icon.color: "transparent"}
            MenuItem{action: casingMenuSetColor3; icon.source: "/Images/CasingColors3.png"; icon.color: "transparent"}
            MenuItem{action: casingMenuSetColor4; icon.source: "/Images/CasingColors4.png"; icon.color: "transparent"}
            MenuItem{action: casingMenuSetColor5; icon.source: "/Images/CasingColors5.png"; icon.color: "transparent"}
            MenuItem{action: casingMenuSetColor6; icon.source: "/Images/CasingColors6.png"; icon.color: "transparent"}
        }

        MenuSeparator{}
        MenuItem{action: casingMenuDelete}
        MenuSeparator{}
        MenuItem{action: casingMenuCopyState}
        MenuItem{action: casingMenuPasteState}
        Connections
        {
            target: corkboard
            function onCasingMenuActiveChanged(active)
            {
                if(active)
                {
                    casingMenu.x = corkboard.casingMenuPt.x - 20
                    casingMenu.y = corkboard.casingMenuPt.y - 180;
                    casingMenu.open();
                }
            }
        }
        onClosed:
        {
            corkboard.setCasingMenuActive(false);
        }
    }

}
