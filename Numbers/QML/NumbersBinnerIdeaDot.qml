import QtQuick 2.0

Rectangle
{
    id: root
    property double finalX: 0
    property double finalY: 0

    width: 4
    height: 4
    radius: 4
    color: Qt.rgba(0.7+0.3*Math.random(1),0.5*Math.random(1),0.5*Math.random(1),1.0)
    opacity: 0.0

    function startAnimating()
    {
        animation.start();
    }

    SequentialAnimation
    {
        id: animation;
        running: false

        NumberAnimation
        {
            target: root
            property: "opacity"
            duration: 1000 * idea.animationSpeed;
            from: 0.0
            to: 1.0
            easing.type: Easing.InOutQuad
        }

        PauseAnimation
        {
            duration: 1200 * idea.animationSpeed;
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: root
                to: finalX - 2;
                property: "x"
                duration: 1000 * idea.animationSpeed;
                easing.type: Easing.Linear
            }
            NumberAnimation
            {
                target: root
                to: finalY - 2
                property: "y"
                duration: 1000 * idea.animationSpeed;
                easing.type: Easing.Linear
            }
        }
        PauseAnimation
        {
            duration: 1200 * idea.animationSpeed;
        }
        NumberAnimation
        {
            target: root
            to: 0.0;
            property: "opacity"
            duration: 1000 * idea.animationSpeed;
            easing.type: Easing.Linear
        }
        ScriptAction
        {
            script: root.destroy();
        }
    }
}
