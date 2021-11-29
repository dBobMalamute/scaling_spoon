import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12

Rectangle
{
    width: 600
    height: 100
    radius: 10
    border.color: Qt.darker("#9f6e41",1.2)
    border.width: 4
    gradient: Gradient
    {
        GradientStop{ position: 0.05; color: "#276436"}
        GradientStop{ position: 0.95; color: "#71A67B"}
    }

    Action
    {
        id: cutAction
        text: "Cut"
        shortcut: "ctrl+x"
        icon.source: "qrc:/Images/editcut.png"
        onTriggered:
        {
           root.width += 10
         //   textArea.cut()
        }
    }

    Action {
        id: copyAction
        text: "Copy"
        shortcut: "Ctrl+C"
        icon.source: "qrc:/Images/editcopy.png"
        onTriggered: textArea.copy()
    }

    Action {
        id: pasteAction
        text: "Paste"
        shortcut: "ctrl+v"
        icon.source: "qrc:/Images/editpaste.png"
        onTriggered: textArea.paste()
    }

    Action {
        id: alignLeftAction
        text: "Left"
        icon.source: "qrc:/Images/textleft.png"
        shortcut: "ctrl+l"
        onTriggered: idea.alignment = Qt.AlignLeft
        checkable: true
        checked: idea.alignment == Qt.AlignLeft
    }
    Action {
        id: alignCenterAction
        text: "Center"
        icon.source: "qrc:/Images/textcenter.png"
        onTriggered: idea.alignment = Qt.AlignHCenter
        checkable: true
        checked: idea.alignment == Qt.AlignHCenter
    }
    Action {
        id: alignRightAction
        text: "Right"
        icon.source: "qrc:/Images/textright.png"
        onTriggered: idea.alignment = Qt.AlignRight
        checkable: true
        checked: idea.alignment == Qt.AlignRight
    }
    Action {
        id: alignJustifyAction
        text: "Justify"
        icon.source: "qrc:/Images/textjustify.png"
        onTriggered: idea.alignment = Qt.AlignJustify
        checkable: true
        checked: idea.alignment == Qt.AlignJustify
    }

    Action {
        id: boldAction
        text: "Bold"
        icon.source: "qrc:/Images/textbold.png"
        onTriggered: idea.bold = !idea.bold
        checkable: true
        checked: idea.bold
    }

    Action {
        id: italicAction
        text: "Italic"
        icon.source: "qrc:/Images/textitalic.png"
        onTriggered: idea.italic = !idea.italic
        checkable: true
        checked: idea.italic
    }
    Action {
        id: underlineAction
        text: "Underline"
        icon.source: "qrc:/Images/textunder.png"
        onTriggered: idea.underline = !idea.underline
        checkable: true
        checked: idea.underline
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["Text files (*.txt)", "HTML files (*.html, *.htm)"]
        onAccepted: {
            if (fileDialog.selectExisting)
                idea.fileUrl = fileUrl
//            else
//                idea.saveAs(fileUrl, selectedNameFilter)
        }
    }

    ColorDialog {
        id: colorDialog
        color: "black"
    }

    Action {
        id: fileOpenAction
        icon.source: "images/fileopen.png"
        text: "Open"
        onTriggered: {
            fileDialog.selectExisting = true
            fileDialog.open()
        }
    }

    MenuBar
    {
        anchors.top: parent.top
        anchors.margins: 5
        Menu {
            title: "&File"
            MenuItem { action: fileOpenAction }
            MenuItem { action: fileSaveAsAction }
            MenuItem { text: "Quit"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "&Edit"
            MenuItem { action: copyAction }
            MenuItem { action: cutAction }
            MenuItem { action: pasteAction }
        }
        Menu {
            title: "F&ormat"
            MenuItem { action: boldAction }
            MenuItem { action: italicAction }
            MenuItem { action: underlineAction }
            MenuSeparator {}
            MenuItem { action: alignLeftAction }
            MenuItem { action: alignCenterAction }
            MenuItem { action: alignRightAction }
            MenuItem { action: alignJustifyAction }
            MenuSeparator {}
            MenuItem {
                text: "&Color ..."
                onTriggered: {
                    colorDialog.color = idea.textColor
                    colorDialog.open()
                }
            }
        }
        Menu {
            title: "&Help"
            MenuItem { text: "About..." ; onTriggered: aboutBox.open() }
        }
    }

    ToolBar
    {
        anchors.bottom: parent.bottom
        anchors.margins: 5
        RowLayout
        {
            anchors.fill: parent
            ToolButton { action: copyAction; display: AbstractButton.IconOnly}
            ToolButton { action: cutAction; display: AbstractButton.IconOnly }
            ToolButton { action: pasteAction; display: AbstractButton.IconOnly }

            ToolButton { action: boldAction; display: AbstractButton.IconOnly }
            ToolButton { action: italicAction; display: AbstractButton.IconOnly }
            ToolButton { action: underlineAction; display: AbstractButton.IconOnly }

            ToolButton { action: alignLeftAction; display: AbstractButton.IconOnly }
            ToolButton { action: alignCenterAction; display: AbstractButton.IconOnly }
            ToolButton { action: alignRightAction; display: AbstractButton.IconOnly }
            ToolButton { action: alignJustifyAction; display: AbstractButton.IconOnly }
            Rectangle
            {
                width: 30
                height: 10
                color: "transparent"
            }
        }


    }

    Image
    {
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.margins: 5
        source: "qrc:/Images/CloseIcon.png"
        width: 20
        height: 20
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                idea.deleteExternalQML();
            }
        }
    }

}
