import QtQuick 2.12
import QtQuick.Controls 2.13

Item
{
    height: 68
    width: Math.max(radianDegreeItem.width, paperArea.width);

    Rectangle
    {
        id: paperArea
        width: Math.max(Math.max(input.width, output.width) + 10, 50);
        height: 34
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
                input.text = "tan(" + inputNumber.toPrecision(3) + ")";
                output.text = "= " + outputNumber.toPrecision(3);
            }
            function onClearNumbers()
            {
                input.text = "";
                output.text = "= ";
            }
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
            id: output

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
    }

    Item
    {
        id: radianDegreeItem
        anchors.horizontalCenter: parent.horizontalCenter

        y: 38;
        width: element.width - 20
        height: 30

        Switch
        {
            id: element
            text: ""
            anchors.horizontalCenter: parent.horizontalCenter
            y: -10
            scale: 0.5
            onToggled:
            {
                //Checked == Radians.
                if(checked)
                {
                    text.text = "Radian"
                    idea.setDegreeMode(false);
                }
                else
                {
                    text.text = "Degree"
                    idea.setDegreeMode(true);
                }
            }
            Connections
            {
                target: idea
                function onDegreeModeChanged(degreeMode)
                {
                    if(degreeMode === false)
                    {
                        element.checked = true;
                        text.text = "Radian";
                    }
                    else
                    {
                        element.checked = false;
                        text.text = "Degree"
                    }
                }
            }
        }
        Text
        {
            id: text
            y: 15
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: -10
            text: "Degree"
        }
    }
}


