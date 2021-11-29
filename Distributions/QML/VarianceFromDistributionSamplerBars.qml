import QtQuick 2.12
import QtGraphicalEffects 1.12

Rectangle
{
    id: root
    width: 2
    height: 120
    color: "red"
    opacity: 0.0

    property real stdevLength: 0

    SequentialAnimation
    {
        id: animation

        PauseAnimation
        {
            duration: 2800*idea.animationSpeed
        }

        NumberAnimation
        {
            target: root
            property: "opacity"
            to: 1.0
            easing.type: Easing.InOutQuad
            duration: 1500*idea.animationSpeed;
        }

        PauseAnimation
        {
            duration: 3600 * idea.animationSpeed;
        }
        NumberAnimation
        {
            targets: [stdevLine1, stdevLine2]
            property: "opacity"
            to: 1.0;
            duration: 1300* idea.animationSpeed;
        }

        PauseAnimation
        {
            duration: 1000 * idea.animationSpeed;
        }

        NumberAnimation
        {
            target: root
            property: "opacity"
            to: 0.0
            easing.type: Easing.InOutQuad
            duration: 1500*idea.animationSpeed;
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

    RectangularGlow
    {
        id: effect
        x: 0
        y: 0
        width: parent.width
        height: parent.height

        glowRadius: 6
        spread: 0.4
        color: "#de1616"
        opacity: 0.1
    }

    Rectangle
    {
        id: stdevLine1
        y: 15;
        x: 1
        height: width
        color: "transparent"
        border.color: "red"
        border.width: 1
        width: stdevLength;
        opacity: 0.0

        RectangularGlow
        {
            x: 0
            y: 0
            width: parent.width
            height: parent.height

            glowRadius: 6
            spread: 0.4
            color: "#de1616"
            opacity: 0.1
        }
    }

    Rectangle
    {
        id: stdevLine2
        y: 15;
        x: -stdevLength + 1
        height: width
        color: "transparent"
        border.color: "red"
        border.width: 1
        width: stdevLength;
        opacity: 0.0

        RectangularGlow
        {
            x: 0
            y: 0
            width: parent.width
            height: parent.height

            glowRadius: 6
            spread: 0.4
            color: "#de1616"
            opacity: 0.1
        }
    }

}
