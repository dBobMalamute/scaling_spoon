import QtQuick 2.12

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
    }

    TextInput
    {
        id: textInput
        font.bold: true
        font.pointSize: 12
        verticalAlignment: TextInput.AlignVCenter
        horizontalAlignment: TextInput.AlignHCenter
        color: "#1a1a1a"
        selectByMouse: true
        readOnly: true
        focus: true

        text: ""

        Connections
        {
            target: idea
            function onDisplayNums(value)
            {
                textInput.text = value;
            }
            function onClearNums()
            {
                textInput.text = "";
            }
        }

        Keys.onPressed:
        {
            event.accepted = true;
            if(event.key === Qt.Key_C && event.modifiers & Qt.ControlModifier)
            {
                idea.sendTextToClipboard(textInput.text);
            }
            if(event.key === Qt.Key_X && event.modifiers & Qt.ControlModifier)
            {
                idea.sendTextToClipboard(textInput.text);
            }
        }

        onActiveFocusChanged:
        {
            if(activeFocus)
                textInput.selectAll();
            else
                textInput.deselect();
        }
    }
}
