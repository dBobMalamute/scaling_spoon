import QtQuick 2.12
import QtQuick.Controls 2.12

FocusScope
{
    width: Math.max(10,textInput.implicitWidth)
    height: textInput.implicitHeight

    Rectangle
    {
        anchors.fill: parent
        anchors.margins: -2

        radius: 4

        gradient: Gradient
        {
            GradientStop{position: 0.0; color: "#ead0b4"}
            GradientStop{position: 1.0; color: "#ddb284"}
        }

        MouseArea
        {
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor;
            acceptedButtons: Qt.NoButton
        }

        TextInput
        {
            id: textInput

            anchors.centerIn: parent
            font.bold: true
            font.pointSize: 12
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            color: "#1a1a1a"
            selectByMouse: true
            validator: DoubleValidator{decimals: 10}
            text: idea.number;
            focus: true

            onEditingFinished:
            {
                idea.setNumber(text)
            }
            onActiveFocusChanged:
            {
                if(textInput.activeFocus && textInput.text === "0")
                {
                    textInput.text = "";
                }
                if(!textInput.activeFocus && textInput.text === "")
                {
                    textInput.text = "0"
                    idea.setNumber(0);
                }
            }

            Connections
            {
                target: idea
                function onNumberChanged()
                {
                    textInput.text = idea.number;
                }
            }
        }
    }
}



