import QtQuick 2.12
import QtQuick.Controls 2.12
import com.malamute.controlsStyles 1.0

Rectangle
{
    width: 140
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
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            Label
            {
                anchors.verticalCenter: parent.verticalCenter
                text: "Bold"
                color: ControlsStyle.textColor();
                font: ControlsStyle.textFont();
            }
            Switch
            {
                anchors.verticalCenter: parent.verticalCenter
                onClicked:
                {
                    idea.bold = checked
                }
                Component.onCompleted:
                {
                    checked = idea.bold
                }
            }
        }
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            Label
            {
                anchors.verticalCenter: parent.verticalCenter
                text: "Size: "
                color: ControlsStyle.textColor();
                font: ControlsStyle.textFont();

            }
            TextInput
            {
                id: textInput
                anchors.verticalCenter: parent.verticalCenter
                validator: IntValidator{bottom: 1; top: 100}
                selectByMouse: true;
                text: idea.fontSize
                color: ControlsStyle.textColor();
                font: ControlsStyle.textFont();

                Connections
                {
                    target: textInput
                    function onEditingFinished()
                    {
                        idea.fontSize = textInput.text;
                    }
                }

            }
        }
    }

    Image
    {
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.margins: 5
        source: "qrc:/Images/CloseIcon.png"
        width: 14
        height: 14
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                idea.deleteExternalQML(0);
            }
        }
    }

}
