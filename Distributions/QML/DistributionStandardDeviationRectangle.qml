import QtQuick 2.0

Item
{
    id: root
    width: 1
    height: 1
    opacity: 1.0


    property real lineDistance: 0;
    property double stdevLength: 0;
    property double intermediateY: 0;

    property color rectColor:  i % 2 === 0 ? Qt.rgba(1.0,0.2*Math.random(1)+0.1,0.2*Math.random(1)+0.1,1.0) :
                                             Qt.rgba(0.7,0.4*Math.random(1)+0.1,0.4*Math.random(1)+0.1,1.0)
    property int i: 0;
    property int sections: 0;

    function startAnimation()
    {
        var yFinal = root.height / 2 - (lineDistance > 0 ? lineDistance / 2 : lineDistance / -2);
        var heightFinal = lineDistance > 0 ? lineDistance : -1.0*lineDistance;

        if(yFinal< 0)
            intermediateY = root.height - heightFinal;
        else
            intermediateY = yFinal;

        animation.start();
    }

    Rectangle
    {
        id: distributionSectionRect
        anchors.fill: parent
        color: rectColor
        opacity: 0.001
    }

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
        id: square;
        y: root.height / 2 - 1;
        height: 1;
        width: 1
        x: root.width / 2 - 1;
        color: "transparent";
        border.color: dot.color
        border.width: 2
        opacity: 0.0
    }

    SequentialAnimation
    {
        id: animation;
        running: false;

        PauseAnimation
        {
            duration: 2000 * i / sections * idea.animationSpeed;
        }

        NumberAnimation
        {
            target: distributionSectionRect
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
        NumberAnimation
        {
            target: distributionSectionRect
            property: "opacity"
            to: 0.10;
            duration: 500*idea.animationSpeed;
        }
        PauseAnimation
        {
            duration: 200*idea.animationSpeed;
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: square
                property: "opacity"
                to: 0.6;
                duration: 1000*idea.animationSpeed;
            }

            NumberAnimation
            {
                target: square
                property: "x"
                to: lineDistance > 0 ? root.width/2 : root.width/2 + (lineDistance);
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
                to: lineDistance > 0 ? root.width / 2 + lineDistance - stdevLength : root.width / 2 + (lineDistance);
                duration: 2000 * idea.animationSpeed
            }
            NumberAnimation
            {
                target: square
                property: "y"
                to: intermediateY + ((lineDistance > 0 ? lineDistance : -1.0*lineDistance) - stdevLength) / 2;
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
        NumberAnimation
        {
            target: distributionSectionRect
            property: "opacity"
            to: 0;
            duration: 500*idea.animationSpeed;
        }
        NumberAnimation
        {
            target: dot
            property: "opacity"
            to: 0;
            duration: 500*idea.animationSpeed;
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
}
