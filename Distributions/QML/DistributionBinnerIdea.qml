import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Shapes 1.12

Item
{
    width: 530
    height: 402

    Rectangle
    {
        width: 530
        height: 330
        radius: 3
        color: "#050512"
    }

    Image
    {
        id: gridImage
        x: 50
        y: 5
        width: 460
        height: 310
        source: "qrc:/Images/GridLines.png"
    }

    Item
    {
        id: yAxisLabels;
        Text
        {
            x: 47 - width
            y: 10 - height/2
            text: idea.gridBounds[3].toPrecision(3)
            color: "#80b3ff"
        }
        Text
        {
            x: 47 - width
            y: 85 - height/2
            text: (0.75*idea.gridBounds[3] + 0.25*idea.gridBounds[2]).toPrecision(3)
            color: "#80b3ff"
        }
        Text
        {
            x: 47 - width
            y: 160 - height/2
            text: (0.5*idea.gridBounds[3] + 0.5*idea.gridBounds[2]).toPrecision(3)
            color: "#80b3ff"
        }
        Text
        {
            x: 47 - width
            y: 235 - height/2
            text: (0.25*idea.gridBounds[3] + 0.75*idea.gridBounds[2]).toPrecision(3)
            color: "#80b3ff"
        }
        Text
        {
            x: 47 - width
            y: 310 - height/2
            text: idea.gridBounds[2].toPrecision(3)
            color: "#80b3ff"
        }

        SequentialAnimation
        {
            id: yAxisLabelsAnimation;
            PauseAnimation
            {
                duration: 3200 * idea.animationSpeed;
            }

            NumberAnimation
            {
                target: yAxisLabels
                property: "opacity"
                from: 0
                to: 1
                duration: 500 * idea.animationSpeed;
            }
        }
    }

    Text
    {
        y: 313
        x: 50 + 5 - width/2
        text: idea.gridBounds[0].toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 313
        x: 112.5 + 50 + 5 -width/2
        text: (0.75*idea.gridBounds[0] + 0.25*idea.gridBounds[1]).toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 313
        x: 225 + 50 + 5 -width/2
        text: (0.5*idea.gridBounds[0] + 0.5*idea.gridBounds[1]).toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 313
        x: 337.5 + 50 + 5 -width/2
        text: (0.25*idea.gridBounds[0] + 0.75*idea.gridBounds[1]).toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 313
        x: 450 + 50 + 5  -width/2
        text: idea.gridBounds[1].toPrecision(3)
        color: "#80b3ff"
    }

    Item
    {
        id: boundsLineArea
        x: 55;
        y: 10;
        width: 450;
        height: 300
    }

    Item
    {
        id: barsArea
        x: 55;
        y: 10;
        width: 450
        height: 300
    }

    Item
    {
        id: plotArea
        anchors.centerIn: gridImage
        width: 450
        height: 300
        clip: true

        Shape
        {
            id: plotShape
            anchors.fill: parent
            visible: idea.valid;

            ShapePath
            {
                id: plotShapePath
                fillColor: "transparent"
                strokeColor: "#1de21d"
                strokeWidth: 2

                startX: 50;
                startY: 50;
            }
        }
    }

    Rectangle
    {
        id: discretePlotArea
        anchors.centerIn: gridImage
        width: 450
        height: 300
        color: "transparent"
        clip: true
    }

    Connections
    {
        target: idea
        function onPlotContinuous(xValues, yValues, finalYValues)
        {
            clear();
            if(xValues.length === 0)
                return;

            yAxisLabels.opacity = 0.0;
            yAxisLabelsAnimation.start();

            const xMin = idea.gridBounds[0];
            const xMax = idea.gridBounds[1];
            const xLength = xMax - xMin;

            const yMin = idea.gridBounds[2];
            const yMax = idea.gridBounds[3];
            const yLength = yMax - yMin;

            //Start the shape path in the correct location.
            plotShapePath.startX = 450 * (xValues[0] - xMin) / xLength;
            plotShapePath.startY = 300 - (280 * yValues[0]);
            for(var i = 1; i < xValues.length; i++)
            {
                const x = 450 * (xValues[i] - xMin) / xLength;
                const y = 300 - (280 * yValues[i]);
                var pathComponent = Qt.createComponent("DistributionBinnerPathLine.qml");
                var pathLine = pathComponent.createObject(plotShapePath, {x: x, y: y});

                pathLine.y = 300 - (300 * (finalYValues[i] - yMin) / yLength);
                plotShapePath.pathElements.push(pathLine);
            }

            for(i = 0; i < idea.binBounds.length; i++)
            {
                var comp = Qt.createComponent("DistributionBinnerIdeaBoundsLine.qml");
                var obj = comp.createObject(boundsLineArea);
                obj.position = idea.getPositionForBoundLine(i);
                obj.startAnimating();
            }

            for(i = 0; i < idea.binBounds.length + 1; i++)
            {
                comp = Qt.createComponent("DistributionBinnerIdeaBar.qml");
                obj = comp.createObject(barsArea);
                obj.x = idea.getPositionForBoundLine(i - 1) + 1;
                obj.width = idea.getBinWidth(i);
                obj.finalHeight = 300 * idea.getBinHeight(i) / (idea.gridBounds[3]-idea.gridBounds[2]);
                obj.moreGreen = i % 2;
                obj.i = i;
                obj.startAnimating();
            }
        }

        function onPlotDiscrete(xValues, yValues)
        {
            clear();
            const xMin = idea.gridBounds[0];
            const xMax = idea.gridBounds[1];
            const xLength = xMax - xMin;

            const yMin = idea.gridBounds[2];
            const yMax = idea.gridBounds[3];
            const yLength = yMax - yMin;

            var comp;
            var obj;
            for(var i = 0; i < xValues.length - 1; i++)
            {
                comp = Qt.createComponent("DistributionBinnerDiscreteDot.qml");
                obj = comp.createObject(discretePlotArea,
                                        {x: 450 * (xValues[i] - xMin) / xLength - 3,
                                         y: (300 - (300 * (yValues[i] - yMin) / yLength) - 3)})
                obj.startAnimating();
            }
            comp = Qt.createComponent("DistributionBinnerDiscreteDot.qml");
            obj = comp.createObject(discretePlotArea,
                                    {x: 450 * (xValues[i] - xMin) / xLength - 3,
                                     y: (300 - (300 * (yValues[i] - yMin) / yLength) - 3)})
            obj.startAnimating();

            for(i = 0; i < idea.binBounds.length; i++)
            {
                comp = Qt.createComponent("DistributionBinnerIdeaBoundsLine.qml");
                obj = comp.createObject(boundsLineArea);
                obj.position = idea.getPositionForBoundLine(i);
                obj.startAnimating();
            }

            for(i = 0; i < idea.binBounds.length + 1; i++)
            {
                comp = Qt.createComponent("DistributionBinnerIdeaBar.qml");
                obj = comp.createObject(barsArea);
                obj.x = idea.getPositionForBoundLine(i - 1) + 1;
                obj.width = idea.getBinWidth(i);
                obj.finalHeight = 300 * idea.getBinHeight(i) / (idea.gridBounds[3]-idea.gridBounds[2]);
                obj.moreGreen = i % 2;
                obj.i = i;
                obj.startAnimating();
            }
        }

        function clear()
        {
            plotShapePath.pathElements = [];

            var i;
            //Clear the bounds lines.
            for(i = 0; i < boundsLineArea.children.length; i++)
            {
                boundsLineArea.children[i].destroy();
            }

            //Clear the bars
            for(i = 0; i < barsArea.children.length; i++)
            {
                barsArea.children[i].destroy();
            }

            //Clear the discretePlotArea.
            for(i = 0; i < discretePlotArea.children.length; i++)
            {
                discretePlotArea.children[i].destroy();
            }
        }
    }

    Rectangle
    {
        width: parent.width
        height: 36
        anchors.bottom: controlRow.top
        anchors.bottomMargin: 3
        radius: 3

        gradient: Gradient
        {
            GradientStop{position: 0.0; color: "#101010"}
            GradientStop{position: 1.0; color: "#303030"}
        }

        clip: true

        ScrollView
        {
            anchors.fill: parent
            anchors.margins: 3

            TextInput
            {
                id: boundsInput
                anchors.fill: parent
                text: "0.00, 1.00, 2.00, 3.00, 4.00";
                font.pointSize: 10
                color: "#7c650d"
                Connections
                {
                    target: idea
                    function onBinBoundsChanged(bounds)
                    {
                        var boundsString = "" + bounds[0].toPrecision(3);
                        var i = 0;
                        for(i = 1; i < bounds.length; i++)
                        {
                            boundsString = boundsString.concat(", ");
                            boundsString = boundsString.concat(bounds[i].toPrecision(3));
                        }
                        boundsInput.text = boundsString;
                    }
                }
                onEditingFinished:
                {
                    idea.sanitizeInputBounds(text);
                }

                Component.onCompleted:
                {
                    var bounds = idea.binBounds;
                    var boundsString = "" + bounds[0].toPrecision(3);
                    var i = 0;
                    for(i = 1; i < bounds.length; i++)
                    {
                        boundsString = boundsString.concat(", ");
                        boundsString = boundsString.concat(bounds[i].toPrecision(3));
                    }
                    boundsInput.text = boundsString;
                }
            }
        }
    }


    Rectangle
    {
        id: controlRowBackground
        anchors.fill: controlRow
        anchors.rightMargin: -5
        anchors.leftMargin: -5
        radius: 3
        color: "#342b3b"
    }

    Row
    {
        id: controlRow
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        spacing: 3;

        Text
        {
            text: "Low: "
            font.bold: true
            font.pointSize: 10
            color: "#b5b5b5"
            anchors.verticalCenter: parent.verticalCenter
        }
        TextInput
        {
            id: lowTextInput
            text: idea.lowBinBoundary
            font.bold: true
            font.pointSize: 10
            color: "#b5b5b5"
            selectByMouse: true;
            validator: DoubleValidator{decimals: 3}
            anchors.verticalCenter: parent.verticalCenter
            onEditingFinished:
            {
                idea.setLowBinBoundary(text);
            }
        }

        Rectangle
        {
            id: spacer
            width: 10
            height: 10
            color: "transparent"
        }

        Text
        {
            text: "Up: ";
            font.bold: true
            font.pointSize: 10
            color: "#b5b5b5"
            anchors.verticalCenter: parent.verticalCenter
        }
        TextInput
        {
            id: upTextInput
            text: idea.upBinBoundary
            font.bold: true
            font.pointSize: 10
            color: "#b5b5b5"
            validator: DoubleValidator{decimals: 3}
            anchors.verticalCenter: parent.verticalCenter
            selectByMouse: true;
            onEditingFinished:
            {
                idea.setUpBinBoundary(text);
            }
        }

        Rectangle
        {
            id: spacer2
            width: 10
            height: 10
            color: "transparent"
        }

        Text
        {
            text: "#: "
            font.bold: true
            font.pointSize: 10
            color: "#b5b5b5"
            anchors.verticalCenter: parent.verticalCenter
        }
        TextInput
        {
            text: idea.numBins
            font.bold: true
            font.pointSize: 10
            color: "#b5b5b5"
            validator: IntValidator{bottom: 2; top: 99}
            selectByMouse: true;
            anchors.verticalCenter: parent.verticalCenter
            onEditingFinished:
            {
                idea.setNumBins(text);
            }
        }

        Rectangle
        {
            id: spacer3
            width: 10
            height: 10
            color: "transparent"
        }

        Button
        {
            text: "Set";
            height: 30
            width: 40
            enabled: idea.upBinBoundary > idea.lowBinBoundary
            onClicked:
            {
                idea.setBoundsByButton();
            }
        }

        Rectangle
        {
            width: 50
            height: 10
            color: "transparent"
        }

        Button
        {
            text: "Replay"
            height: 30
            width: 80
            onClicked:
            {
                idea.replay();
            }
        }
    }
}
