import QtQuick 2.12

Rectangle
{
    height: 50
    radius: 4
    width: Math.max(Math.max(input.width + 14, output1.width, output2.width) + 10, 50)

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
            squareRootImage.visible = true;
            input.text = "" + inputNumber.toPrecision(3);
            output1.text = "= " + outputNumber.toPrecision(3);
            output2.text = "= " + (-1.0*outputNumber).toPrecision(3);
        }
        function onClearNumbers()
        {
            squareRootImage.visible = false;
            input.text = "";
            output1.text = "= ";
            output2.text = "= ";
        }
    }

    Image
    {
        id: squareRootImage
        visible: false
        source: "qrc:/Images/SquareRootImage.png"
        height: 16
        y: 1
        fillMode: Image.PreserveAspectFit

        anchors.right: input.left
        anchors.rightMargin: -22
    }

    TextInput
    {
        id: input;
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
        id: output1;

        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: input.bottom

        font.bold: true
        font.pointSize: 10
        color: "#1a1a1a"
        selectByMouse: true
        readOnly: true

        text: "= "
    }

    TextInput
    {
        id: output2

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
