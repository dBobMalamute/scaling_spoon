import QtQuick 2.0

FocusScope
{
    width: textInput.width
    height: textInput.height

    TextInput
    {
        id: textInput
        visible: idea.valid;
        font.bold: true
        font.pointSize: 12
        verticalAlignment: TextInput.AlignVCenter
        horizontalAlignment: TextInput.AlignHCenter
        color: "#1a1a1a"
        selectByMouse: true
        readOnly: true
        focus: true;

        text: idea.mean.toPrecision(3);

        Rectangle
        {
            anchors.fill: parent
            anchors.margins: -3
            border.color: "#1a1a1a"
            border.width: 2
            radius: 2
            color: "#e6e6e6"
            z: -1
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
