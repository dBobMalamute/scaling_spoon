import QtQuick 2.0

Item
{
    id: root
    width: 1
    height: 1
    opacity: 1.0
    Rectangle
    {
        id: rect
        anchors.fill: parent
        color: rectColor
        opacity: 0.001
    }

    property real lineDistance: 0;
    property color rectColor:  i % 2 === 0 ? Qt.rgba(1.0,0.2*Math.random(1)+0.1,0.2*Math.random(1)+0.1,1.0) :
                                             Qt.rgba(0.7,0.4*Math.random(1)+0.1,0.4*Math.random(1)+0.1,1.0)
    property int i: 0;
    property int sections: 0;

    Rectangle
    {
        id: dot;

        x: parent.width / 2 - 3;
        y: parent.height / 2 - 3;
        z: 2

        width: 6
        height: 6
        radius: 6
        opacity: 0.0

        color: Qt.lighter(rectColor, 1.3);
    }

    Rectangle
    {
        id: bar;
        y: rect.height / 2 - 1;
        height: 2
        width: 0;
        x: parent.width / 2;
        z: 1
        color: Qt.lighter(rectColor, 1.3);
        opacity: 0.8;
    }

    SequentialAnimation
    {
        id: animation;
        running: true;

        PauseAnimation
        {
            duration: 2000 * i / sections * idea.animationSpeed;
        }

        NumberAnimation
        {
            target: rect
            property: "opacity"
            to: 0.45;
            duration: 500*idea.animationSpeed;
        }
        PauseAnimation
        {
            duration: 2000 * (sections - i) / sections * idea.animationSpeed;
        }

        PauseAnimation
        {
            duration: 200*idea.animationSpeed;
        }
        PropertyAction
        {
            target: dot;
            property: "opacity"
            value: 1.0;
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: dot;
                properties: "width,height"
                to: 4
                duration: 1500*idea.animationSpeed;
            }
            NumberAnimation
            {
                target: dot
                property: "y"
                to: dot.y + 1;
                duration: 1500*idea.animationSpeed;
            }
            NumberAnimation
            {
                target: dot
                property: "x"
                to: dot.x + 1;
                duration: 1500*idea.animationSpeed;
            }
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
                to: lineDistance > 0 ? rect.width / 2 : rect.width / 2 + lineDistance;
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
