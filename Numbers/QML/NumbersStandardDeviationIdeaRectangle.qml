import QtQuick 2.0

Item
{
    id: root
    property double lineDistance: 0;
    property double stdevLength: 0;
    property double intermediateY: 0;
    x: 0;

    function startAnimation()
    {
        var yFinal = root.y + 3 - (lineDistance > 0 ? lineDistance / 2 : lineDistance / -2);
        var heightFinal = lineDistance > 0 ? lineDistance : -1.0*lineDistance;

        if(yFinal< 0)
            intermediateY = -root.y;
        else if(yFinal + heightFinal > 480)
        {
            intermediateY = 480 - root.y - heightFinal;
        }
        else
            intermediateY = yFinal - root.y;

        animation.start();
    }

    SequentialAnimation
    {
        id: animation
        PauseAnimation
        {
            duration: 1000 * idea.animationSpeed
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: dot
                properties: "width,height"
                to: 6
                duration: 1000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: dot
                property: "x"
                to: -3
                duration: 1000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: dot
                property: "y"
                to: 3
                duration: 1000 * idea.animationSpeed
            }
        }
        PauseAnimation
        {
            duration: 1100 * idea.animationSpeed
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: square
                property: "x"
                to: lineDistance > 0 ? 0 : (lineDistance);
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
            duration: 1000 * idea.animationSpeed
        }

        ParallelAnimation
        {
            NumberAnimation
            {
                target: square
                property: "x"
                to: lineDistance > 0 ? lineDistance - stdevLength : (lineDistance);
                duration: 2000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: square
                property: "y"
                to: square.y + (square.height - stdevLength) / 2;
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
            duration: 2000 * idea.animationSpeed
        }
        NumberAnimation
        {
            target: square
            property: "opacity"
            to: 0
            duration: 2000 * idea.animationSpeed
        }
    }

    Rectangle
    {
        id: dot
        width: 12
        height: 12
        radius: 6
        x: - 6;

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
        opacity: 0.6
    }
}


