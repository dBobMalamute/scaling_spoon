import QtQuick 2.12

FocusScope
{
    id: scope
    width: 100
    height: rectangle.height

    Rectangle
    {
        id: rectangle
        width: samplesText.implicitWidth + numberText.implicitWidth + 10;
        height: samplesText.implicitHeight + 10
        radius: 3
        color: "#bd9173"
        border.color: "#1a1a1a"
        border.width: 2

        Row
        {
            id: row
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            Text
            {
                id: samplesText
                text: "Samples: "
                font.bold: true
                font.pointSize: 10
                color: "#1a1a1a"
            }
            TextInput
            {
                id: numberText
                text: idea.totalSamples;
                font.bold: true
                font.pointSize: 10
                font.underline: true
                focus: true
                selectByMouse: true
                color: "#1a1a1a"
                validator: IntValidator{bottom: 0; top: 99999}
                onEditingFinished:
                {
                    idea.setTotalSamples(text);
                }

            }
        }
        Image
        {
            anchors.right: parent.right
            anchors.rightMargin: -10
            source: "qrc:/Images/Pencil.png"
            fillMode: Image.PreserveAspectFit
            width: 10

        }
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            scope.focus = true;
        }
        onDoubleClicked:
        {
            numberText.selectAll();
        }
    }
}
