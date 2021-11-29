import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtQuick.Controls 2.12

Rectangle
{
    width: 530
    height: 330
    radius: 3
    color: "#050512"

    property bool displayContinuous : false;

    Image
    {
        id: gridImage
        x: 50
        y: 5
        width: 460
        height: 310
        source: "qrc:/Images/GridLines.png"
    }

    TextInput
    {
        validator: DoubleValidator{}
        x: 47 - width
        y: 10 - height/2
        text: distributionDisplay.yHigh.toPrecision(3);
        color: "#80b3ff"
        selectByMouse: true
        onEditingFinished:
        {
            idea.setBound(text, 3);
        }
    }
    Text
    {
        x: 47 - width
        y: 85 - height/2
        text: (0.75*distributionDisplay.yHigh + 0.25*distributionDisplay.yLow).toPrecision(3)
        color: "#80b3ff"
    }
    Text
    {
        x: 47 - width
        y: 160 - height/2
        text: (0.5*distributionDisplay.yHigh + 0.5*distributionDisplay.yLow).toPrecision(3)
        color: "#80b3ff"
    }
    Text
    {
        x: 47 - width
        y: 235 - height/2
        text: (0.25*distributionDisplay.yHigh + 0.75*distributionDisplay.yLow).toPrecision(3)
        color: "#80b3ff"
    }
    TextInput
    {
        validator: DoubleValidator{}
        x: 47 - width
        y: 310 - height/2
        text: distributionDisplay.yLow.toPrecision(3)
        color: "#80b3ff"
        selectByMouse: true
        onEditingFinished:
        {
            idea.setBound(text, 2);
        }
    }
    TextInput
    {
        validator: DoubleValidator{}
        y: 313
        x: 50 + 5 - width/2
        text: distributionDisplay.xLow.toPrecision(3)
        color: "#80b3ff"
        selectByMouse: true
        onEditingFinished:
        {
            idea.setBound(text, 0);
        }
    }

    Text
    {
        y: 313
        x: 112.5 + 50 + 5 -width/2
        text: (0.75*distributionDisplay.xLow + 0.25*distributionDisplay.xHigh).toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 313
        x: 225 + 50 + 5 -width/2
        text: (0.5*distributionDisplay.xLow + 0.5*distributionDisplay.xHigh).toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 313
        x: 337.5 + 50 + 5 -width/2
        text: (0.25*distributionDisplay.xLow + 0.75*distributionDisplay.xHigh).toPrecision(3)
        color: "#80b3ff"
    }

    TextInput
    {
        validator: DoubleValidator{}
        y: 313
        x: 450 + 50 + 5  -width/2
        text: distributionDisplay.xHigh.toPrecision(3)
        color: "#80b3ff"
        selectByMouse: true
        onEditingFinished:
        {
            idea.setBound(text, 1);
        }
    }

    CheckBox
    {
        anchors.top: parent.bottom
        anchors.right: parent.right
        text: "Fit Axis";
        checked: distributionDisplay.autoFit ? true : false;
        onCheckedChanged:
        {
            idea.setAutoFit(checked);
        }
    }

    Item
    {
        id: continuousPlotArea
        anchors.centerIn: gridImage
        width: 450
        height: 300
        clip: true
        visible: idea.valid && displayContinuous;

        Shape
        {
            id: plotLine
            anchors.fill: parent

            ShapePath
            {
                id: plotShapePath
                fillColor: "transparent"
                strokeColor: "#59a72f"
                strokeWidth: 2

                startX: 0;
                startY: 300;
            }
        }
    }
    Item
    {
        id: discretePlotArea
        anchors.centerIn: gridImage
        width: 450
        height: 300
        clip: true
        visible: idea.valid && !displayContinuous;
    }



    function clear()
    {
        // Empty the shape path.
        plotShapePath.pathElements = [];

        //Clear the discretePlotArea.
        for(var i = 0; i < discretePlotArea.children.length; i++)
        {
            discretePlotArea.children[i].destroy();
        }
    }

    Connections
    {
        target: distributionDisplay;

        function onDisplayContinuousDistribution(xValues, yValues)
        {
            displayContinuous = true;
            clear();

            //Start the shape path in the correct location.
            plotShapePath.startX = xValues[0];
            plotShapePath.startY = yValues[0];

            for(var i = 1; i < xValues.length; i++)
            {
                var pathLine = Qt.createQmlObject("import QtQuick 2.12; import QtQuick.Shapes 1.12; PathLine{}",
                                                  plotShapePath);
                pathLine.x = xValues[i];
                pathLine.y = yValues[i];

                plotShapePath.pathElements.push(pathLine);
            }
        }
        function onDisplayDiscreteDistribution(xValues, yValues)
        {
            displayContinuous = false;
            clear()

            var comp;
            var obj;
            var i = 0
            for(i = 0; i < xValues.length - 1; i++)
            {
                comp = Qt.createComponent("DistributionDisplayDiscreteDot.qml");
                obj = comp.createObject(discretePlotArea,
                                        {x: xValues[i] - 3,
                                         y: yValues[i] - 3,
                                         horizLineLength: (xValues[i+1] - xValues[i])})
            }
            comp = Qt.createComponent("DistributionDisplayDiscreteDot.qml");
            obj = comp.createObject(discretePlotArea,
                                    {x: xValues[i] - 3,
                                     y: yValues[i] - 3,
                                     horizLineLength: (xValues[i] - xValues[i-1])})
        }
    }
}
