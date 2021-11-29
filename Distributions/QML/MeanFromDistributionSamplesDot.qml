import QtQuick 2.0

Rectangle
{
    id: root
    width: 4
    height: 4
    radius: 2
    opacity: 0.0
    color: Qt.rgba(1.0,0.4*Math.random(1)+0.3,0.4*Math.random(1)+0.3,1)

    property real dotFinalX: 0;
    property real dotFinalY: 0;
    property real barFinalX: 0;
    property real barFinalWidth: 0;

    Rectangle
    {
        id: bar
        x: 0
        y: 1
        width: 1
        height: 2
        color: parent.color;
    }

    SequentialAnimation
    {
        id: animation

        PauseAnimation
        {
            duration: 200*idea.animationSpeed
        }

        ParallelAnimation
        {
            NumberAnimation
            {
                target: root
                property: "x"
                duration: 3000*idea.animationSpeed
                easing.type: Easing.Linear
                to: dotFinalX - 2
            }
            NumberAnimation
            {
                target: root
                property: "y"
                duration: 3000*idea.animationSpeed
                easing.type: Easing.Linear
                to: dotFinalY - 2
            }
        }

        PauseAnimation
        {
            duration: 200*idea.animationSpeed
        }

        ParallelAnimation
        {
            NumberAnimation
            {
                target: bar
                property: "width"
                duration: 3000*idea.animationSpeed
                easing.type: Easing.Linear
                to: barFinalWidth;
            }
            NumberAnimation
            {
                target: bar
                property: "x"
                duration: 3000*idea.animationSpeed
                easing.type: Easing.Linear
                to: barFinalX;
            }
        }

        PauseAnimation
        {
            duration: 1500*idea.animationSpeed
        }
        NumberAnimation
        {
            target: root
            property: "opacity"
            to: 0;
            duration: 2000*idea.animationSpeed
        }
        ScriptAction
        {
            script: reset();
        }
    }

    function startAnimating(i, x, y, mean)
    {
        root.x = (i % 2) * 450
        root.y = 0;
        root.opacity = 1.0;
        root.dotFinalX = x;
        root.dotFinalY = y;
        root.barFinalWidth = Math.abs(mean - x);
        root.barFinalX = Math.min(2, mean - (x-2))

        animation.start();
    }
    function reset()
    {
        animation.stop();
        bar.x = 0;
        bar.width = 1;
        root.opacity = 0.0;
    }
}
