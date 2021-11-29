import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Shapes 1.12

import com.distributions.dists 1.0

Item
{
    width: 530
    height: 360

    DistributionDisplay
    {
        id: item_distributionDisplay
    }

    Item
    {
        id: meanLineArea
        x: 55
        y: 10
        width: 450
        height: 300
        clip: true

        Shape
        {
            id: meanLine
            anchors.fill: parent
            visible: idea.options[1] && idea.valid

            ShapePath
            {
                id: meanShapePath
                fillColor: "transparent"
                strokeColor: "#40aebf"
                strokeWidth: 2

                strokeStyle: ShapePath.DashLine
                dashPattern: [20, 8];

                startX: 450*(idea.mean - distributionDisplay.xLow) / (distributionDisplay.xHigh - distributionDisplay.xLow)
                startY: 0;
                PathLine
                {
                    id: meanPathLine
                    x: 450*(idea.mean - distributionDisplay.xLow) / (distributionDisplay.xHigh - distributionDisplay.xLow)
                    y: 300
                }
            }
            Label
            {
                text: "Mean"
                y: 10
                x: meanShapePath.startX + 2
                color: "#40aebf"
                font.bold: true
                font.pointSize: 12

                visible: meanShapePath.visible
                ToolTip.text: "Mean: " + idea.mean.toFixed(3);
                ToolTip.visible: meanMouseArea.containsMouse;
                background: Rectangle
                {
                    anchors.fill: parent
                    radius: 2
                    color: "black"
                    z: -1
                }
                MouseArea
                {
                    id: meanMouseArea
                    anchors.fill: parent
                    hoverEnabled: true;
                }
            }
        }
    }

    Row
    {
        anchors.left: parent.left
        anchors.top: item_distributionDisplay.bottom

        CheckBox
        {
            text: "Show Mean";
            checked: idea.options[1];
            onCheckedChanged:
            {
                idea.setOption(checked, 1)
            }
        }       
    }
}
