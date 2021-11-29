import QtQuick 2.12
import QtQuick.Controls 2.12

import "PowerHelperFunctions.js" as HelperFunctions

Rectangle
{
    width: 620
    height: 360
    color: "#42535a"

    property real inputNumber: 0.0;
    property real outputNumber: 0.0;
    property real currentlyShownBoxes: 1.0;
    property real multiplications: 0.0;
    property real fraction: 0.0;

    Connections
    {
        target: idea
        function onDisplayNumbers(input, output)
        {
            HelperFunctions.clearBoxes();
            inputNumber = input;
            outputNumber = output;
            currentlyShownBoxes = 1.0;
            multiplications = 0.0;
            fraction = input % 1;

            inputText.text = "x" + input.toFixed(2);
            if(output <= 10000 && input >= 0)
            {
                HelperFunctions.setupForBoxes(output);
                multiplyByTenButton.enabled = true
                resetButton.enabled = true
                animationSwitch.enabled = true;
                outputText.text = 1.0;
                animationUnsupportedText.visible = false;
                if(animationSwitch.checked)
                    animationTimer.start();
            }
            else
            {
                multiplyByTenButton.enabled = false;
                animationSwitch.enabled = false;
                resetButton.enabled = false;
                outputText.text = outputNumber.toFixed(3)
                animationUnsupportedText.visible = true;
            }
        }
        function onClearNumbers()
        {
            animationUnsupportedText.visible = false;
            inputText.text = "";
            outputText.text = "";
            currentlyShownBoxes = 0.0;
            multiplications = 0.0;
            fraction = 0.0;
            HelperFunctions.clearBoxes();
        }
    }

    Rectangle
    {
        id: inputArea
        anchors.left: parent.left
        height: parent.height
        width: 100
        gradient: Gradient
        {
            GradientStop{position: 0.0; color: "black"}
            GradientStop{position: 0.5; color: "#282828"}
            GradientStop{position: 1.0; color: "black"}
        }

        Image
        {
            id: inputImage
            anchors.centerIn: parent
            source: "qrc:/Images/1to10Picture.png"
        }

        Text
        {
            anchors.top: inputImage.bottom
            anchors.topMargin: 4
            id: inputText
            text: "";
            color: "#e8e8e8"
            font.pointSize: 10
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle
    {
        id: outputArea
        anchors.right: parent.right
        height: parent.height
        width: 100
        gradient: Gradient
        {
            GradientStop{position: 0.0; color: "black"}
            GradientStop{position: 0.5; color: "#282828"}
            GradientStop{position: 1.0; color: "black"}
        }

        Rectangle
        {
            id: outputBox
            anchors.centerIn: parent
            width: 80
            height: 60
            gradient: Gradient
            {
                GradientStop{position: 0.2; color: "#2a7fff"}
                GradientStop{position: 0.5; color: "#ff6600"}
                GradientStop{position: 0.8; color: "#2a7fff"}
            }
            border.width: 2
            radius: 5
            border.color: "black"
        }


        Text
        {
            id: outputText
            anchors.top: outputBox.bottom
            anchors.topMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
            y: 190
            color: "#e8e8e8"
            font.pointSize: 10
        }
    }

    Rectangle
    {
        id: animationArea
        y: 4
        width: 400
        anchors.horizontalCenter: parent.horizontalCenter
        height: 300
        color: "#1a1a1a"
    }

    Item
    {
        id: fractionAnimationArea
        anchors.fill: animationArea;
    }

    Text
    {
        id: animationUnsupportedText;
        text: "Animation Unsupported";
        horizontalAlignment: Text.AlignHCenter
        color: "white"
        font.bold: true;
        font.pointSize: 10
        anchors.centerIn: animationArea
        visible: false
    }

    function multiplyByTen()
    {
        if(10*currentlyShownBoxes <= outputNumber)
        {
            currentlyShownBoxes *= 10;
            multiplications += 1;
            outputText.text = currentlyShownBoxes.toFixed(0);
            HelperFunctions.addBoxes(currentlyShownBoxes);
        }
        else
        {
            currentlyShownBoxes = outputNumber;
            HelperFunctions.finishAddingBoxes(currentlyShownBoxes);
            outputText.text = outputNumber.toFixed(3);
            animationTimer.stop();
        }
    }
        Timer
        {
            id: animationTimer;
            interval: 3000;
            repeat: true;
            onTriggered:
            {
                multiplyByTen();
            }
        }

    Rectangle
    {
        id: controlsRow
        width: 326
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        height: 46

        radius: 4
        color: "#0e58c7"
        Row
        {
            x:10
            spacing: 6
            Button
            {
                id: multiplyByTenButton
                text: "Multiply by 10"
                anchors.verticalCenter: parent.verticalCenter;
                onClicked:
                {
                    multiplyByTen();
                }
            }

            Switch
            {
                id: animationSwitch;
                text: "Animate"
                anchors.verticalCenter: parent.verticalCenter;
                onClicked:
                {
                    if(checked)
                        multiplyByTen();
                    animationTimer.running = checked;
                }
            }
            Button
            {
                id: resetButton
                text: "Reset"
                anchors.verticalCenter: parent.verticalCenter;
                onClicked:
                {
                    animationTimer.stop();
                    HelperFunctions.clearBoxes();
                    currentlyShownBoxes = 1;
                    multiplications = 0.0;
                    outputText.text = "1.0";
                    HelperFunctions.setupForBoxes(outputNumber);
                    if(animationSwitch.checked)
                        animationTimer.start();
                }
            }
        }
    }
}
