import QtQuick 2.12
import QtQuick.Controls 2.12

FocusScope
{
    id: root
    width: 30+Math.max(10, numeratorInput.implicitWidth, denominatorInput.implicitWidth)
    height: 5+numeratorInput.implicitHeight - 4 + denominatorInput.implicitHeight;

    Rectangle
    {
        anchors.fill: parent
        anchors.margins: -2

        radius: 4

        gradient: Gradient
        {
            GradientStop{position: 0.0; color: "#ead0b4"}
            GradientStop{position: 1.0; color: "#ddb284"}
        }

        MouseArea
        {
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor;
            acceptedButtons: Qt.NoButton
        }
        Item
        {
            x: 20
            height: root.height - 5;
            width: root.width - 20;
            y: 5
            TextInput
            {
                id: numeratorInput

                anchors.horizontalCenter: parent.horizontalCenter
                font.bold: true
                font.pointSize: 12
                verticalAlignment: TextInput.AlignVCenter
                horizontalAlignment: TextInput.AlignHCenter
                color: "#1a1a1a"
                selectByMouse: true
                validator: DoubleValidator{decimals: 10}
                text: idea.numerator;
                focus: true

                onEditingFinished:
                {
                    idea.setNumerator(text)
                }
            }

            Rectangle
            {
                anchors.centerIn: parent
                height: 1
                width: parent.width - 4
                color: "black"
            }

            TextInput
            {
                id: denominatorInput

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                font.bold: true
                font.pointSize: 12
                verticalAlignment: TextInput.AlignVCenter
                horizontalAlignment: TextInput.AlignHCenter
                color: "#1a1a1a"
                selectByMouse: true
                validator: DoubleValidator{decimals: 10}
                text: idea.denominator;
                focus: true

                onEditingFinished:
                {
                    idea.setDenominator(text)
                }
            }
        }


    }
    Image
    {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        source: "qrc:/Images/TallSquareRootImage.png"
    }
}



