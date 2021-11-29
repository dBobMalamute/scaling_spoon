import QtQuick 2.0

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
    text: idea.valid ? idea.mean.toPrecision(4) : ""
}
