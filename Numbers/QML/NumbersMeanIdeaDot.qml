import QtQuick 2.0

Item
{
    id: root
    property double lineDistance: 0;
    x: 0;

    function startAnimation()
    {
        dot.width = 6;
        dot.height = 6
        bar.width = lineDistance > 0 ? lineDistance : -1.0*lineDistance;
        bar.x = lineDistance > 0 ? 0 : (lineDistance);
        dot.y += 3;
        dot.x = -3
    }

    Rectangle
    {
        id: dot
        width: 12
        height: 12
        radius: 6
        x: - 6;

        color: Qt.rgba(1.0,0.4*Math.random(1)+0.3,0.4*Math.random(1)+0.3,1)

        Behavior on width
        {
            SequentialAnimation
            {
                PauseAnimation
                {
                    duration: 2000 * idea.animationSpeed
                }
                NumberAnimation
                {
                    duration: 1000 * idea.animationSpeed
                }
            }
        }
        Behavior on height
        {
            SequentialAnimation
            {
                PauseAnimation
                {
                    duration: 2000 * idea.animationSpeed
                }
                NumberAnimation
                {
                    duration: 1000 * idea.animationSpeed
                }
            }
        }

        Behavior on x
        {
            SequentialAnimation
            {
                PauseAnimation
                {
                    duration: 2000 * idea.animationSpeed
                }
                NumberAnimation
                {
                    duration: 1000 * idea.animationSpeed
                }
            }
        }
        Behavior on y
        {
            SequentialAnimation
            {
                PauseAnimation
                {
                    duration: 2000 * idea.animationSpeed
                }
                NumberAnimation
                {
                    duration: 1000 * idea.animationSpeed
                }
            }
        }
    }

    Rectangle
    {
        id: bar;
        y: dot.height / 2 + 2;
        height: 2;
        width: 1
        x: 0;
        color: dot.color;
        opacity: 0.4

        Behavior on x
        {
            SequentialAnimation
            {
                PauseAnimation
                {
                    duration: 3000 * idea.animationSpeed
                }
                NumberAnimation
                {
                    duration: 3000 * idea.animationSpeed
                }
            }
        }
        Behavior on width
        {
            SequentialAnimation
            {

                PauseAnimation
                {
                    duration: 3000 * idea.animationSpeed
                }
                NumberAnimation
                {
                    duration: 3000 * idea.animationSpeed
                }
            }
        }
    }
}


