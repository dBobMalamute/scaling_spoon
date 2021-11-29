import QtQuick 2.12
import QtQuick.Controls 2.12

ToolBar
{
    id: palleteBar
    height: 60
    background: Rectangle
    {
        anchors.fill: parent
        color: "#C19A6B";
        border.color: Qt.darker(color,1.3);
        border.width: 2
    }
    ComboBox
    {
        id: ideaCollectionComboBox
        editable: false
        width: 250
        height: 60

        model: malamuteHandler.collectionList
        onActivated:
        {
            malamuteHandler.setPalleteList(currentValue)
        }
        font.bold: true
        font.pixelSize: 16
        background: Rectangle
        {
            anchors.fill: parent
            color: "#C19A6B"
            border.color: Qt.darker(color,1.3);
            border.width: 2
        }

        ToolTip.visible: hovered
        ToolTip.delay: 2000
        ToolTip.text: "Choose a new Idea Collection"
        Component.onCompleted:
        {
            ideaCollectionComboBox.currentIndex = 0;
            malamuteHandler.setPalleteList(currentValue);
        }
    }

    ListView
    {
        id: palleteListView
        height: parent.height
        interactive: false
        clip: true
        anchors.left: ideaCollectionComboBox.right
        anchors.leftMargin: 2
        anchors.right: parent.right
        orientation: ListView.Horizontal
        model: malamuteHandler.paletteList
        spacing: 3
        delegate: PaletteItem
        {

        }
        ScrollBar.horizontal: ScrollBar
        {
            id: palleteScrollBar
        }
    }
}
