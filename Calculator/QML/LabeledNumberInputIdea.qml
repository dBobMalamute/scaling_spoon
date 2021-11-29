import QtQuick 2.12

FocusScope
{
    id: root
    height: textInput.height + numInput.height
    width: textInput.width > numInput.width ? textInput.width : numInput.width;

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
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.pointSize: 12
        verticalAlignment: TextInput.AlignVCenter
        horizontalAlignment: TextInput.AlignHCenter
        color: "#1a1a1a"
        selectByMouse: true
        text: idea.text
        focus: true;

        onEditingFinished:
        {
            idea.setText(textInput.text)
        }

        onActiveFocusChanged:
        {
            if(textInput.activeFocus && textInput.text === "Label")
            {
                textInput.text = "";
            }
            if(!textInput.activeFocus && textInput.text === "")
            {
                textInput.text = "Label"
            }
        }

        KeyNavigation.tab: numInput;
        KeyNavigation.down: numInput;

        Connections
        {
            target: idea
            function onTextChanged()
            {
                textInput.text = idea.text;
            }
        }
        MouseArea
        {
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor;
            acceptedButtons: Qt.NoButton
        }
    }

    TextInput
    {
        id: numInput
        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.pointSize: 12
        verticalAlignment: TextInput.AlignVCenter
        horizontalAlignment: TextInput.AlignHCenter
        color: "#1a1a1a"
        selectByMouse: true
        validator: DoubleValidator{decimals: 10}
        text: idea.number

        onActiveFocusChanged:
        {
            if(numInput.activeFocus && numInput.text === "0")
            {
                numInput.text = "";
            }
            if(!numInput.activeFocus && numInput.text === "")
            {
                numInput.text = "0"
                idea.setNumber(0);
            }
        }

        KeyNavigation.tab: textInput;
        KeyNavigation.up: textInput;

        onEditingFinished:
        {
            idea.setNumber(numInput.text)
        }
        Connections
        {
            target: idea
            function onNumberChanged()
            {
                numInput.text = idea.number;
            }
        }
        MouseArea
        {
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor;
            acceptedButtons: Qt.NoButton
        }
    }
}


