import QtQuick 2.0

Rectangle
{
    id: root
    property double position: 0;

    y: 0
    height: 1
    opacity: 1.0
    width: 2
    x: position - 1
    color: "#7f670d"

    function startAnimating()
    {
        animation.start();
    }

    SequentialAnimation
    {
        id: animation;
        running: false

        PauseAnimation
        {
            duration: 1100 * idea.animationSpeed;
        }

        NumberAnimation
        {
            target: root
            property: "height"
            duration: 1000 * idea.animationSpeed;
            from: 0.0;
            to: 300
            easing.type: Easing.Linear;
        }

        PauseAnimation
        {
            duration: 2300 * idea.animationSpeed;
        }

        NumberAnimation
        {
            target: root
            property: "opacity"
            duration: 1000 * idea.animationSpeed;
            from: 1.0
            to: 0.0
            easing.type: Easing.InOutQuad
        }

        ScriptAction
        {
            script: root.destroy();
        }
    }
}
