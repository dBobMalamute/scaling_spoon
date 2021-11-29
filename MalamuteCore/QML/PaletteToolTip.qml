import QtQuick 2.0


Rectangle
{
    property alias text : textEdit.text;

    visible: false;
    z: 100
    width: textEdit.implicitWidth + 20;
    height: textEdit.implicitHeight + 16;
    radius: 15

    border.color: "black"
    border.width: 2

    gradient: Gradient
    {
        GradientStop{position: 0.00; color: "#36828d"}
        GradientStop{position: 0.20; color: "#68f2d2"}
        GradientStop{position: 0.80; color: "#68f2d2"}
        GradientStop{position: 1.00; color: "#36828d"}
    }

    TextEdit
    {
        id: textEdit
        anchors.centerIn: parent
        font.bold: true
        font.pointSize: 15
    }

    Behavior on visible
    {
        NumberAnimation
        {
            duration: 1000
        }
    }
}
