import QtQuick 2.0
import QtQuick.Controls 2.13

Item
{
    id: root
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

