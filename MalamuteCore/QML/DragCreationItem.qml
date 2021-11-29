import QtQuick 2.0

Item
{
    width: 50
    height: 50
    z: 1000
    property string iconSource: "qrc:/Blank.png"


    Rectangle
    {
        x: 0
        y: 0
        width: 55; height: 55
        radius: 5

        color: "#303030"
    }

    Image
    {
        anchors.centerIn: parent
        width: 50
        height: 50
        source: iconSource
    }


}
