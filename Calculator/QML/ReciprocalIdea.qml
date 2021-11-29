import QtQuick 2.12

Rectangle
{
    height: 50
    radius: 4
    width: Math.max(Math.max(input.width , output.width) + 10, 50)

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
            input.text = "÷ " + inputNumber.toPrecision(3);
            output.text = "= " + outputNumber.toPrecision(3);
        }
        function onClearNumbers()
        {
            input.text = "÷ ";
            output.text = "= ";
        }
    }

    TextInput
    {
        id: boxForTheOne
        anchors.right: parent.right
        anchors.rightMargin: 5

        font.bold: true
        font.pointSize: 10
        color: "#1a1a1a"
        selectByMouse: true
        readOnly: true

        text: "1.000"
    }

    TextInput
    {
        id: input;

        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: boxForTheOne.bottom

        font.bold: true
        font.pointSize: 10
        font.underline: true
        color: "#1a1a1a"
        selectByMouse: true
        readOnly: true

        text: "÷ "
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
