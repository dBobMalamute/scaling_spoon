import QtQuick 2.0

Rectangle
{
    id: rect
    width: 2
    height: 100
    color: Qt.rgba(1.0,0.4*Math.random(1)+0.3,0.4*Math.random(1)+0.3,1)
    opacity: 0.01;

    property real lineDistance: 0;

    Rectangle
    {
        id: bar
        y: parent.height / 4
        height: parent.height / 2
        width: 1
        x: 0
        color: parent.color
        opacity: 0.4
    }

    SequentialAnimation
    {
        id: animation
        running: true;


        PauseAnimation
        {
            duration: 2000*idea.animationSpeed;
        }

        NumberAnimation
        {
            target: rect
            property: "opacity"
            to: 1.0;
            duration: 2000*idea.animationSpeed;
        }
        PauseAnimation
        {
            duration: 200*idea.animationSpeed;
        }

        ParallelAnimation
        {
            NumberAnimation
            {
                target: bar;
                property: "x";
                to: lineDistance > 0 ? 0 : lineDistance;
                duration: 3000*idea.animationSpeed;
            }
            NumberAnimation
            {
                target: bar;
                property: "width";
                to: lineDistance > 0 ? lineDistance : -1.0* lineDistance;
                duration: 3000*idea.animationSpeed;
            }
        }
    }
}
