import QtQuick 2.12
import QtQuick.Controls 2.12
import com.malamute.controlsStyles 1.0

Rectangle
{
    width: 100
    height: 80

    radius: ControlsStyle.radius()
    border.color: ControlsStyle.borderColor()
    border.width: ControlsStyle.borderWidth()

    gradient: Gradient
    {
        GradientStop{ position: 0.05; color: ControlsStyle.gradientTop()}
        GradientStop{position: 0.5; color: ControlsStyle.gradientMid()}
        GradientStop{ position: 0.95; color: ControlsStyle.gradientBottom()}
    }


    Column
    {
        anchors.fill: parent
        anchors.margins: 10
        Text
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Speed"
            color: ControlsStyle.textColor();
            font: ControlsStyle.textFont();
        }
        Slider
        {
            width: parent.width
            from: -3
            to: 1
            value: -1.0 * Math.log(idea.animationSpeed)

            onMoved:
            {
                idea.setAnimationSpeed(Math.exp(-1.0*value));
            }
        }
    }
}
