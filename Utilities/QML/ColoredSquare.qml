import QtQuick 2.12
import com.testing.test 1.0

Rectangle
{
    width: 100
    height: 100
    color: "teal"

    LoadTestingRectangle
    {
        rad: 12
        anchors.centerIn: parent
    }

}
