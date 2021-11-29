import QtQuick 2.0

Rectangle
{
    id: root

    property int borderWidth: 2;

    width: 60
    height: 40
    gradient: Gradient
    {
        GradientStop{position: 0.2; color: "#2a7fff"}
        GradientStop{position: 0.5; color: "#ff6600"}
        GradientStop{position: 0.8; color: "#2a7fff"}
    }
    border.width: borderWidth
    radius: height/8
    border.color: "black"

    function fadeOut()
    {
        fadeOutAnimation.start();
    }
    SequentialAnimation
    {
        id: fadeOutAnimation

        NumberAnimation
        {
            target: root
            property: "opacity"
            to: 0.0;
            duration: 2500
        }
        ScriptAction
        {
            script: root.destroy();
        }
    }
}
