import QtQuick 2.15

Rectangle
{
    id: root
    width: 2
    height: 20
    opacity: 0.0

    property real finalY: 0

    color: Qt.rgba(0.3*Math.random(1)+0.3,0.7*Math.random(1)+0.3,0.7*Math.random(1)+0.3,0.7)

    SequentialAnimation
    {
        id: animation

        PauseAnimation
        {
            duration: 6400*idea.animationSpeed
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
                to: finalY;
                duration: 5000 * idea.animationSpeed;
            }
            NumberAnimation
            {
                target: root
                properties: "height"
                to: 4
                duration: 5000 * idea.animationSpeed;
            }
        }
    }

    function startAnimation()
    {
        animation.start();
    }
}
