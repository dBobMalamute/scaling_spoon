import QtQuick 2.0

Rectangle
{
    id: dot

    width: 6
    height: 6
    radius: 3
    color: "green"

    Rectangle
    {
        id: bar;
        color: "green"
        width: 2
        height: 1
        x: 2
        y: 3
    }

    SequentialAnimation
    {
        id: animation;
        PauseAnimation
        {
            duration: 2200 * idea.animationSpeed;
        }
        NumberAnimation
        {
            target: bar
            property: "height"
            to: 300 - dot.y - 2
            duration: 1000 * idea.animationSpeed;
            easing.type: Easing.InOutQuad
        }
        PauseAnimation
        {
            duration: 1100 * idea.animationSpeed;
        }

        NumberAnimation
        {
            target: dot
            property: "opacity"
            duration: 1000 * idea.animationSpeed;
            from: 1.0
            to: 0.0
            easing.type: Easing.InOutQuad
        }
        ScriptAction
        {
            script: dot.destroy();
        }

    }
    function startAnimating()
    {
        animation.start();
    }
}
