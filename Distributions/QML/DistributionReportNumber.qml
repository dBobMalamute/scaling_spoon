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
    text: ""


    Connections
    {
        target: idea
        function onDisplayNumber(value)
        {
            textInput.text = value.toPrecision(4);
        }
        function onClearDisplay()
        {
            textInput.text = "";
        }
    }
}
