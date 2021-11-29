import QtQuick 2.15

Item
{
    id: root
    width: 2
    height: 2
    opacity: 0.0

    property real finalY: 0
    property real stdevLength: 0


    SequentialAnimation
    {
        id: animation

        PauseAnimation
        {
            duration: 9100*idea.animationSpeed
        }

        NumberAnimation
        {
            target: root
            property: "opacity"
            to: 1
            easing.type: Easing.InOutQuad
            duration: 1500*idea.animationSpeed;
        }

        ParallelAnimation
        {
            NumberAnimation
            {
                target: root
                property: "y"
                to: finalY - stdevLength;
                duration: 5000 * idea.animationSpeed;
            }
        }
    }

    Rectangle
    {
        id: stdevLine1
        height: width
        x: 1
        width: stdevLength - 1;
        border.color: Qt.rgba(0.3*Math.random(1)+0.3,0.7*Math.random(1)+0.3,0.7*Math.random(1)+0.3,0.7)
        border.width: 1
        color: "transparent"
    }
    Rectangle
    {
        id: stdevLine2
        height: width
        width: stdevLength - 1
        x: -stdevLength;
        border.color: stdevLine1.border.color;
        border.width: 1
        color: "transparent"
    }

    function startAnimation()
    {
        animation.start();
    }
}
