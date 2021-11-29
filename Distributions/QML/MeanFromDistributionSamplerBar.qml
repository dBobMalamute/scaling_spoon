import QtQuick 2.0

Rectangle
{
    id: root
    width: 2
    height: 100
    color: "red"
    opacity: 0.0

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
            to: 1.0
            easing.type: Easing.InOutQuad
            duration: 1500*idea.animationSpeed;
        }

        NumberAnimation
        {
            target: root
            property: "opacity"
            to: 0
            easing.type: Easing.InOutQuad
            duration: 2000*idea.animationSpeed;
        }

        ScriptAction
        {
            script: root.destroy();
        }

    }

    function startAnimating()
    {
        animation.start();
    }
}
