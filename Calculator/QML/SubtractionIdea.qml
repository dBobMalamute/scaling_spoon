import QtQuick 2.12

Rectangle
{
    color: "Teal"
    height: 50
    radius: 4
    width: Math.max(Math.max(input1.width, input2.width, output.width) + 10, 50)

    gradient: Gradient
    {
        GradientStop{position: 0.0; color: "#ead0b4"}
        GradientStop{position: 1.0; color: "#ddb284"}
    }

    Connections
    {
        target: idea
        function onDisplayNumbers(inputNumber1, inputNumber2, outputNumber)
        {
            input1.text = inputNumber1.toPrecision(3);
            input2.text = "- " + inputNumber2.toPrecision(3);
            output.text = "= " + outputNumber.toPrecision(3);
        }
        function onClearNumbers()
        {
            input1.text = "";
            input2.text = "- ";
            output.text = "= ";
        }
    }

    TextInput
    {
        id: input1;
        anchors.right: parent.right
        anchors.rightMargin: 5

        font.bold: true
        font.pointSize: 10
        color: "#1a1a1a"
        selectByMouse: true
        readOnly: true

        text: ""
    }

    TextInput
    {
        id: input2;

        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: input1.bottom

        font.bold: true
        font.pointSize: 10
        font.underline: true
        color: "#1a1a1a"
        selectByMouse: true
        readOnly: true

        text: "- "
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
