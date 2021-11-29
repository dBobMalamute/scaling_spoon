import QtQuick 2.12

FocusScope
{
    width: Math.max(10,numsInput.implicitWidth)
    height: numsInput.implicitHeight

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
    }
    TextInput
    {
        id: numsInput
        font.bold: true;
        font.pointSize: 12;
        verticalAlignment: TextInput.AlignVCenter;
        horizontalAlignment: TextInput.AlignHCenter;
        color: "#1a1a1a"
        selectByMouse: true;
        text: idea.getNums();
        focus: true;

        onEditingFinished:
        {
            idea.setNums(text)
        }

        //At first this might seem useless, but it's necessary for
        //undo/redo.
        Connections
        {
            target: idea;
            function onNumsChanged()
            {
                numsInput.text = idea.getNums();
            }
        }
    }
}
