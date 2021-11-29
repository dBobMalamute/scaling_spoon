import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Item
{
    width: 60
    height: 24;//comboBox.height;

    ComboBox
    {
        id: comboBox

        x: -10
        anchors.verticalCenter: parent.verticalCenter;
        property bool complete: false

        editable: false
        flat: true
        width: 82
        model: ["PI", "e", "PI/2", "PI/4", "1/PI", "1/e"];
        onCurrentTextChanged:
        {
            if(complete)
            {
                idea.setIndex(comboBox.currentIndex);
            }
        }

        Component.onCompleted:
        {
            comboBox.currentIndex = idea.getIndex();
            complete = true;
        }

        Connections
        {
            target: idea
            function onIndexChanged(i)
            {
                comboBox.currentIndex = i;
            }
        }
    }
}



