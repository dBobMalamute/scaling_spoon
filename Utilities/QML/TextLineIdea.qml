import QtQuick 2.12

FocusScope
{
    width: textInput.width + 8
    height: textInput.height + 8
    Rectangle
    {
        anchors.fill: parent
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
        anchors.centerIn: parent
        font.bold: idea.bold
        font.pointSize: idea.fontSize
        verticalAlignment: TextInput.AlignVCenter
        horizontalAlignment: TextInput.AlignHCenter
        color: "#1a1a1a"
        selectByMouse: true
        text: idea.text;
        focus: true

        onEditingFinished:
        {
            idea.setText(textInput.text)
        }

        onActiveFocusChanged:
        {
            if(textInput.text === 'texttext' && textInput.focus === true)
                textInput.text = '';
            if(textInput.text === '' && textInput.focus === false)
            {
                textInput.text = 'texttext'
                idea.setText(textInput.text)
            }
        }

        Connections
        {
            target: idea
            function onTextChanged()
            {
                textInput.text = idea.text;
            }
        }
    }
}


