import QtQuick 2.12
import QtQuick.Controls 2.12
import "PaletteBarUtilites.js" as Code


Item
{
    id: paletteItem
    width: 40
    height: 40
    y: 3

    property string iconSource;

    MouseArea
    {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        onDoubleClicked:
        {
            corkboard.createIdea(modelData,Qt.point(applicationWindow.width/4,applicationWindow.height/3))
        }
        onPressed:
        {
            palleteScrollBar.enabled = false
            if(mouse.button == Qt.LeftButton)
            {
                Code.startDrag(mouse, iconSource);
            }
        }
        onPositionChanged:
        {
            Code.continueDrag(mouse);
        }
        onReleased:
        {
            if(mouse.button == Qt.LeftButton)
            {
                if(mouse.y > paletteItem.height)
                {
                    corkboard.createIdea(modelData, mapToItem(palleteBar,mouse.x - 30, mouse.y + 30))
                }
                Code.endDrag(mouse);
            }
            palleteScrollBar.enabled = true
        }


        onHoveredChanged:
        {
            if(containsMouse)
            {
                paletteToolTip.visible = true;
                paletteToolTip.text = modelData;
                paletteToolTip.x = mapToItem(palleteBar, 10, 0).x
                paletteToolTip.y = mapToItem(palleteBar, 10, 0).y
            }
            else
            {
                paletteToolTip.visible = false;
            }
        }
    }

    Image
    {
        anchors.fill: parent
        source: iconSource
    }

    Component.onCompleted:
    {
        iconSource = malamuteHandler.ideaIconPath(modelData);
    }
}
