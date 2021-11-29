import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle
{
    id: root
    property double finalHeight: 0
    property int moreGreen: 0
    property int i : 0;

    width: 20
    height: 1
    y: 300
    color: Qt.rgba(0.2+0.4*(1-moreGreen)*Math.random(1),0.33 + moreGreen*0.5*Math.random(1),0.8,0.75)

    SequentialAnimation
    {
        id: animation
        PauseAnimation
        {
            duration: 3300 * idea.animationSpeed;
        }

        ParallelAnimation
        {
            NumberAnimation
            {
                target: root
                property: "height"
                to: finalHeight
                duration: 1000 * idea.animationSpeed;
            }
            NumberAnimation
            {
                target: root
                property: "y"
                to: 300 - finalHeight;
                duration: 1000 * idea.animationSpeed;
            }
        }
    }

    function startAnimating()
    {
        animation.start();
    }

    Label
    {
        id: label
        anchors.fill: parent

        ToolTip
        {
            parent: label
            x: (parent.width - width) / 2;
            y: (parent.height - height) / 2 - 30;
            visible: mouseArea.containsMouse;
            text: idea.getBinHeight(i).toPrecision(3);
        }
    }
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        anchors.topMargin: -20
        hoverEnabled: true
    }
}
