import QtQuick 2.0

Rectangle
{
    id: border
    property color stopLightColor : "black"

    width: 22
    height: 22
    color: "transparent"
    radius: 20
    border.color: "transparent"
    border.width: 3

    Rectangle
    {
        id: light
        x: 11;
        y: 11;
        width: 0;
        height: 0;
        radius: 10;
        color: stopLightColor
    }

    Component.onCompleted:
    {
        animation.start();
    }

    SequentialAnimation
    {
        id: animation

        PauseAnimation {duration: 700}
        PropertyAction
        {
            target: border
            property: "border.color"
            value: "#222222"
        }
        ParallelAnimation
        {
            PropertyAnimation
            {
                target: light
                properties: "width,height"
                to: 16
                duration: 1000
            }
            PropertyAnimation
            {
                target: light
                properties: "x,y"
                to: 3
                duration: 1000
            }
        }
    }
}
