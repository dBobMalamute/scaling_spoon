import QtQuick 2.12
import QtQuick.Controls 2.12

import "LogarithmHelperFunctions.js" as HelperFunctions

Rectangle
{
    width: 620
    height: 360
    color: "#42535a"

    property real inputNumber: 0.0;
    property real outputNumber: 0.0;
    property real currentlyShownBoxes: 0.0;
    property real divisions: 0.0;
    property real fractionRemaining: 0.0;

    Connections
    {
        target: idea
        function onDisplayNumbers(input, output)
        {
            HelperFunctions.clearBoxes();
            inputNumber = input;
            outputNumber = output;
            currentlyShownBoxes = input;
            divisions = 0.0;
            fractionRemaining = input % 1;

            inputText.text = "x" + input.toFixed(2);
            if(input <= 10000 && input >= 1)
            {
                HelperFunctions.createBoxes(input);
                divideByTwoButton.enabled = true
                resetButton.enabled = true
                animationSwitch.enabled = true;
                outputText.text = 0.0;
                animationUnsupportedText.visible = false;
                if(animationSwitch.checked)
                    animationTimer.start();
            }
            else
            {
                divideByTwoButton.enabled = false;
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
            divisions = 0.0;
            fractionRemaining = 0.0;
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

        Rectangle
        {
            id: inputBox
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
            anchors.top: inputBox.bottom
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

        Image
        {
            id: outputImage
            anchors.centerIn: parent
            source: "qrc:/Images/10to1Picture.png"
        }

        Text
        {
            id: outputText
            anchors.top: outputImage.bottom
            anchors.topMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
            y: 190
            color: "#e8e8e8"
            font.pointSize: 10
            text: divisions.toFixed(3);
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
        id: fractionRemainingAnimationArea
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

    function divideByTen()
    {
        if(currentlyShownBoxes >= 10)
        {
            currentlyShownBoxes /= 10;
            divisions += 1;
            outputText.text = divisions.toFixed(3);
            HelperFunctions.remove(currentlyShownBoxes);
        }
        else
        {
            HelperFunctions.removeFractionBox();
            HelperFunctions.remove(1);
            currentlyShownBoxes = 1.0;
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
            divideByTen();
        }
    }

    Rectangle
    {
        id: controlsRow
        width: 306 //Row is width: 286
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
                id: divideByTwoButton
                text: "Divide by 10"
                anchors.verticalCenter: parent.verticalCenter;
                onClicked:
                {
                    divideByTen();
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
                        divideByTen();
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
                    currentlyShownBoxes = inputNumber;
                    divisions = 0.0;
                    outputText.text = "0.0";
                    HelperFunctions.createBoxes(currentlyShownBoxes);
                    if(animationSwitch.checked)
                        animationTimer.start();
                }
            }
        }
    }
}
