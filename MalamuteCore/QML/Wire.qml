import QtQuick 2.12
import QtQuick.Shapes 1.12

import com.malamute.core 1.0

MalamuteWire
{
    Shape
    {
        id: id_shape;
        anchors.fill: parent

        //Selected Halo
        ShapePath
        {
            strokeWidth: selected ? WireStyle.selectedLineWidth() : 0
            strokeColor: WireStyle.selectedColor()
            fillColor: "transparent"
            startX: outPoint.x
            startY: outPoint.y

            PathCubic
            {
                x: inPoint.x;
                y: inPoint.y;

                control1X: controlPoint1.x;
                control1Y: controlPoint1.y;

                control2X: controlPoint2.x;
                control2Y: controlPoint2.y;
            }
        }

        // Normal or construction path.
        ShapePath
        {
            id: shapePath
            strokeWidth: transmitting ? WireStyle.transmittingLineWidth(): WireStyle.lineWidth();
            strokeColor: transmitting ? Qt.lighter(color, WireStyle.transmittingLightenFactor()) : color;
            fillColor: "transparent"
            startX: outPoint.x
            startY: outPoint.y

            strokeStyle: dragging ? ShapePath.DashLine : ShapePath.SolidLine
            dashPattern: [4, 4]

            Behavior on strokeWidth
            {
                NumberAnimation {duration: WireStyle.transmittingAnimationTime()}
            }

            PathCubic
            {
                x: inPoint.x;
                y: inPoint.y;

                control1X: controlPoint1.x;
                control1Y: controlPoint1.y;

                control2X: controlPoint2.x;
                control2Y: controlPoint2.y;
            }
        }
    }

    //Circle at inPoint
    Rectangle
    {
        x: inPoint.x - WireStyle.endpointSize() / 2
        y: inPoint.y - WireStyle.endpointSize() / 2
        width: WireStyle.endpointSize();
        height: WireStyle.endpointSize();
        radius: WireStyle.endpointSize();
        color: shapePath.strokeColor
    }

    //Circle at outPoint
    Rectangle
    {
        x: outPoint.x - WireStyle.endpointSize() / 2
        y: outPoint.y - WireStyle.endpointSize() / 2
        width: WireStyle.endpointSize();
        height: WireStyle.endpointSize();
        radius: WireStyle.endpointSize();
        color: shapePath.strokeColor
    }
}
