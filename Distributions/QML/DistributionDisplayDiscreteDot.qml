import QtQuick 2.0

Rectangle
{
    id: dot
    property real horizLineLength: 0

    width: 6
    height: 6
    radius: 3
    color: "green"
    Rectangle
    {
        id: bar
        x: 3
        y: 2
        height: 2
        width: 1
        color: parent.color;
        opacity: 0.5
        Behavior on width
        {
            NumberAnimation
            {
                duration: 5000
            }
        }
        Component.onCompleted:
        {
            bar.width = horizLineLength
        }
    }
}
