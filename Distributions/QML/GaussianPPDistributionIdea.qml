import QtQuick 2.12
import QtQuick.Controls 2.12
import QtCharts 2.3
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
        id: plotArea
        x: 55
        y: 10
        width: 450
        height: 300
        clip: true

        Shape
        {
            id: lowLine
            anchors.fill: parent
            visible: idea.options[1] && idea.valid && idea.paramValid[0];

            ShapePath
            {
                id: lowShapePath
                fillColor: "transparent"
                strokeColor: "#40aebf"
                strokeWidth: 2

                strokeStyle: ShapePath.DashLine
                dashPattern: [20, 8];

                startX: 450*(idea.params[0] - distributionDisplay.xLow) / (distributionDisplay.xHigh - distributionDisplay.xLow)
                startY: 0;
                PathLine
                {
                    id: lowPathLine
                    x: 450*(idea.params[0] - distributionDisplay.xLow) / (distributionDisplay.xHigh - distributionDisplay.xLow)
                    y: 300
                }
            }
            Label
            {
                text: "Low"
                y: 10
                x: lowShapePath.startX + 2
                color: "#40aebf"
                font.bold: true
                font.pointSize: 12

                visible: lowShapePath.visible
                ToolTip.text: "Low: " + idea.params[0].toFixed(3);
                ToolTip.visible: lowMouseArea.containsMouse;
                background: Rectangle
                {
                    anchors.fill: parent
                    radius: 2
                    color: "black"
                    z: -1
                }
                MouseArea
                {
                    id: lowMouseArea
                    anchors.fill: parent
                    hoverEnabled: true;
                }
            }
        }

        Shape
        {
            id: upLine
            anchors.fill: parent
            visible: idea.options[2] && idea.valid && idea.paramValid[1];

            ShapePath
            {
                id: upShapePath
                fillColor: "transparent"
                strokeColor: "#40aebf"
                strokeWidth: 2

                strokeStyle: ShapePath.DashLine
                dashPattern: [20, 8];

                startX: 450*(idea.params[1] - distributionDisplay.xLow) / (distributionDisplay.xHigh - distributionDisplay.xLow)
                startY: 0;
                PathLine
                {
                    id: upPathLine
                    x: 450*(idea.params[1] - distributionDisplay.xLow) / (distributionDisplay.xHigh - distributionDisplay.xLow)
                    y: 300
                }
            }
            Label
            {
                text: "Up"
                y: 10
                x: upShapePath.startX + 2
                color: "#40aebf"
                font.bold: true
                font.pointSize: 12

                visible: upShapePath.visible
                ToolTip.text: "Up: " + idea.params[1].toFixed(3);
                ToolTip.visible: upMouseArea.containsMouse;
                background: Rectangle
                {
                    anchors.fill: parent
                    radius: 2
                    color: "black"
                    z: -1
                }
                MouseArea
                {
                    id: upMouseArea
                    anchors.fill: parent
                    hoverEnabled: true;
                }
            }
        }

        Shape
        {
            id: stdevLine
            anchors.fill: parent
            visible: idea.options[4] && idea.valid && idea.paramValid[3];

            ShapePath
            {
                id: stdevShapePath
                fillColor: "transparent"
                strokeColor: "#2c3aba"
                strokeWidth: 2

                startX: meanShapePath.startX - 450*(idea.params[3])
                        / (distributionDisplay.xHigh - distributionDisplay.xLow);
                startY: 120;
                PathLine
                {
                    id: stdevPathLine
                    x: meanShapePath.startX + 450*(idea.params[3])
                       / (distributionDisplay.xHigh - distributionDisplay.xLow);
                    y: 120
                }
            }
            Label
            {
                text: "Stdev"
                x: Math.min((stdevPathLine.x + stdevShapePath.startX) / 2 - width - 2,
                            stdevPathLine.x *0.25 + 0.75*stdevShapePath.startX - width/2)
                y: stdevShapePath.startY - height - 2;
                color: "#2c3aba"
                font.bold: true
                font.pointSize: 12
                visible: stdevShapePath.visible

                ToolTip.text: "Standard Deviation: " + idea.params[3].toFixed(3);
                ToolTip.visible: stdevMouseArea2.containsMouse;
                background: Rectangle
                {
                    anchors.fill: parent
                    radius: 2
                    color: "black"
                    z: -1
                }
                MouseArea
                {
                    id: stdevMouseArea2
                    anchors.fill: parent
                    hoverEnabled: true
                }
            }
            Label
            {
                text: "Stdev"
                x: Math.max((stdevPathLine.x + stdevShapePath.startX) / 2 + 2,
                            stdevPathLine.x *0.75 + 0.25*stdevShapePath.startX - width/2)
                y: stdevShapePath.startY - height - 2;
                color: "#2c3aba"
                font.bold: true
                font.pointSize: 12
                visible: stdevShapePath.visible

                ToolTip.text: "Standard Deviation: " + idea.params[3].toFixed(3);
                ToolTip.visible: stdevMouseArea1.containsMouse;
                background: Rectangle
                {
                    anchors.fill: parent
                    radius: 2
                    color: "black"
                    z: -1
                }
                MouseArea
                {
                    id: stdevMouseArea1
                    anchors.fill: parent
                    hoverEnabled: true
                }
            }
        }

        Shape
        {
            id: meanLine
            anchors.fill: parent
            visible: idea.options[3] && idea.valid && idea.paramValid[2]

            ShapePath
            {
                id: meanShapePath
                fillColor: "transparent"
                strokeColor: "#40aebf"
                strokeWidth: 2

                strokeStyle: ShapePath.DashLine
                dashPattern: [20, 8];

                startX: 450*(idea.params[2] - distributionDisplay.xLow) / (distributionDisplay.xHigh - distributionDisplay.xLow)
                startY: 0;
                PathLine
                {
                    id: meanPathLine
                    x: 450*(idea.params[2] - distributionDisplay.xLow) / (distributionDisplay.xHigh - distributionDisplay.xLow)
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
                ToolTip.text: "Mean: " + idea.params[2].toFixed(3);
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
            text: "Show Low";
            enabled: idea.paramValid[0];
            checked: idea.options[1];
            onCheckedChanged:
            {
                idea.setOption(checked, 1)
            }
        }
        CheckBox
        {
            text: "Show Up";
            enabled: idea.paramValid[1];
            checked: idea.options[2];
            onCheckedChanged:
            {
                idea.setOption(checked, 2)
            }
        }
        CheckBox
        {
            text: "Show Mean";
            enabled: idea.paramValid[2];
            checked: idea.options[3];
            onCheckedChanged:
            {
                idea.setOption(checked, 3)
            }
        }
        CheckBox
        {
            text: "Show Stdev";
            enabled: idea.paramValid[3];
            checked: idea.options[4];
            onCheckedChanged:
            {
                idea.setOption(checked, 4)
            }
        }
    }
}
