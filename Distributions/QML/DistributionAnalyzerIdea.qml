import QtQuick 2.12
import QtQuick.Controls 2.12

Item
{
    id: root
    height: comboBox.height + numDisplay.height;
    width: 80

    ComboBox
    {
        id: comboBox

        property bool complete: false

        flat: true;
        editable: false
        width: 100
        model: ["mean", "median", "mode", "stdev"]

        onCurrentTextChanged:
        {
            if(complete)
            {
                idea.setStatistic(comboBox.currentIndex)
            }
        }

        Component.onCompleted:
        {
            complete = true;
        }

        Connections
        {
            target: idea
            function onStatisticChanged()
            {
                comboBox.currentIndex = idea.statistic;
            }
        }
    }
    TextInput
    {
        id: numDisplay
        anchors.top: comboBox.bottom;
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.pointSize: 12
        verticalAlignment: TextInput.AlignVCenter
        horizontalAlignment: TextInput.AlignHCenter
        color: "white"
        selectByMouse: true
        readOnly: true

        text: "0.0"

        Connections
        {
            target: idea
            function onDisplayNum(value)
            {
                numDisplay.text = value;
            }
        }
    }

}
