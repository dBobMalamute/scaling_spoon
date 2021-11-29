import QtQuick 2.15

Rectangle
{
    width: 8
    height: 8
    radius: 4

    color: Qt.rgba(0.3*Math.random(1)+0.3,0.7*Math.random(1)+0.3,0.7*Math.random(1)+0.3,0.7)

    Behavior on y
    {
        NumberAnimation{duration: 4000}
    }
    Behavior on width
    {
        NumberAnimation{duration: 4000}
    }
    Behavior on height
    {
        NumberAnimation{duration: 4000}
    }
    Behavior on x
    {
        NumberAnimation{duration: 4000}
    }

    Component.onCompleted:
    {
        y += 60 + 220*Math.random();
        width = 4;
        height = 4;
    }
}
