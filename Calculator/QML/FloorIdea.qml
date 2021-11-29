import QtQuick 2.0

Rectangle
{
    height: 34
    width: Math.max(Math.max(input.width, output.width) + 10, 50);
    radius: 4

    gradient: Gradient
    {
        GradientStop{position: 0.0; color: "#ead0b4"}
        GradientStop{position: 1.0; color: "#ddb284"}
    }

    Connections
    {
        target: idea
        function onDisplayNumbers(inputNumber, outputNumber)
        {
            input.text = inputNumber.toPrecision(3) + "\u2193" ;
            output.text = "= " + outputNumber.toPrecision(3);
        }
        function onClearNumbers()
        {
            input.text = "\u2193";
            output.text = "= ";
        }
    }

    Text
    {
        id: input;
        anchors.right: parent.right
        anchors.rightMargin: 5

        font.bold: true
        font.pointSize: 10
        color: "#1a1a1a"
        textFormat: Text.RichText

        text: "\u2193"
    }
    TextInput
    {
        id: output

        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom

        font.bold: true
        font.pointSize: 10
        color: "#1a1a1a"
        selectByMouse: true
        readOnly: true

        text: "= "
    }
}
