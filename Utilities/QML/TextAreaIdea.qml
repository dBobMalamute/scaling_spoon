import QtQuick 2.12

//Note the control is a TextEdit not a TextArea
//This is because I explained the name to someone important before I
//programmed it. I read the manual for QtQuick Controls 1 instead
//of QtQuick Controls 2. My bad. -dBob.

FocusScope
{
    width: textEdit.width + 8
    height: textEdit.height + 8
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
    TextEdit
    {
        id: textEdit
        anchors.centerIn: parent
        font.bold: idea.bold
        font.pointSize: idea.fontSize
        color: "#1a1a1a"
        selectByMouse: true
        text: idea.text

        onEditingFinished:
        {
            idea.setText(textEdit.text)
        }

        onActiveFocusChanged:
        {
            if(textEdit.text === 'text\ntext' && textEdit.focus === true)
                textEdit.text = '';
            if(textEdit.text === '' && textEdit.focus === false)
            {
                textEdit.text = 'text\ntext'
                idea.setText(textEdit.text)
            }
        }

        Connections
        {
            target: idea
            function onTextChanged()
            {
                textEdit.text = idea.text;
            }
        }
    }
}



