import QtQuick 2.0

Item
{
    id: root

    property real dotFinalX: 0;
    property real dotFinalY: 0;

    property real stdevLength: 0;
    property real meanPos: 0;
    property real lineDistance: 0
    property double intermediateY: 0;
    property double squareFinalY: 0

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
                target: dot
                property: "x"
                duration: 3000*idea.animationSpeed
                easing.type: Easing.Linear
                to: dotFinalX - 2
            }
            NumberAnimation
            {
                target: dot
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

        PropertyAction{target: square; properties: "width,height"; value: 1;}
        PropertyAction {target: square; property: "x"; value: dotFinalX;}
        PropertyAction {target: square; property: "y"; value: dotFinalY;}

        ParallelAnimation
        {
            NumberAnimation
            {
                target: square
                property: "opacity"
                to: 0.6
                duration: 500*idea.animationSpeed;
            }

            NumberAnimation
            {
                target: square
                property: "x"
                to: dotFinalX + (lineDistance > 0 ? 0 : (lineDistance));
                duration: 2000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: square
                property: "y"
                to: intermediateY;
                duration: 2000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: square
                property: "width"
                to: lineDistance > 0 ? lineDistance : -1.0*lineDistance;
                duration: 2000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: square
                property: "height"
                to: lineDistance > 0 ? lineDistance : -1.0*lineDistance
                duration: 2000 * idea.animationSpeed
            }
        }

        PauseAnimation
        {
            duration: 500*idea.animationSpeed
        }

        ParallelAnimation
        {
            NumberAnimation
            {
                target: square
                property: "x"
                to: square.x + (lineDistance > 0 ? lineDistance - stdevLength : (lineDistance));
                duration: 2000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: square
                property: "y"
                to: squareFinalY;
                duration: 2000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: square
                properties: "width,height"
                to: stdevLength;
                duration: 2000 * idea.animationSpeed
            }
        }

        PauseAnimation
        {
            duration: 1500*idea.animationSpeed
        }
        NumberAnimation
        {
            targets: [dot, square]
            property: "opacity"
            to: 0;
            duration: 2000*idea.animationSpeed
        }
        ScriptAction
        {
            script: reset();
        }
    }

    function startAnimating(i, x, y, stdevLength, meanPos)
    {
        dot.x = (i % 2) * 450
        dot.y = 0;
        dot.opacity = 1.0;
        root.dotFinalX = x;
        root.dotFinalY = y;
        square.x = dotFinalX;
        square.y = dotFinalY;

        root.stdevLength = stdevLength;
        root.meanPos = meanPos;
        root.lineDistance = meanPos - x;
        root.intermediateY = Math.max(0, dotFinalY - Math.abs(lineDistance/2));
        root.squareFinalY = Math.max(0, dotFinalY - (stdevLength / 2));

        animation.start();
    }
    function reset()
    {
        animation.stop();
        dot.opacity = 0.0;
        square.opacity = 0.0;
    }

    Rectangle
    {
        id: dot
        width: 4
        height: 4
        radius: 2

        color: Qt.rgba(0.8,0.4*Math.random(1)+0.3,0.4*Math.random(1)+0.3,1)
    }

    Rectangle
    {
        id: square;
        y: dot.height / 2 + 2;
        height: 1;
        width: 1
        x: 0;
        color: "transparent";
        border.color: dot.color
        border.width: 2
        opacity: 0.0
    }
}
