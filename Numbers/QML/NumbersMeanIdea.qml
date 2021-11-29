import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4

Item
{
    width: 500
    height: 300

    Rectangle
    {
        width: parent.width
        height: 257
        radius: 3
        color: "#050512"
    }

    Image
    {
        id: gridImage
        x: 20
        y: 0
        width: 460
        height: 237
        source: "qrc:/Images/MeanGrid.png"
        fillMode: Image.PreserveAspectFit
    }

    Rectangle
    {
        id: dotsArea
        anchors.centerIn: gridImage
        width: 450
        height: 227
        color: "transparent"
    }

    Connections
    {
        target: idea
        function onNewDots(numDots)
        {
            var i;
            var comp;
            var obj;

            var meanPos = 0 + 450 * (idea.mean - idea.lowBound)/(idea.upBound - idea.lowBound);

            meanLine.opacity = 0.0
            meanText.opacity = 0.0
            //Clear the display.
            for(i = 0; i < dotsArea.children.length; i++)
            {

                dotsArea.children[i].destroy();
            }

            //Add dots if there are any.
            for(i = 0; i < numDots; i++)
            {
                comp = Qt.createComponent("NumbersMeanIdeaDot.qml");
                obj = comp.createObject(dotsArea)
                obj.x = idea.getPositionForDot(i);
                obj.y = 20.0 + 185.0 * (i / (numDots-1));
                obj.lineDistance = meanPos - obj.x;
                obj.startAnimation();
            }
            if(numDots > 0)
                meanLineAnimation.start();
        }
    }

    SequentialAnimation
    {
        id: meanLineAnimation

        PauseAnimation
        {
            duration: 6300 * idea.animationSpeed
        }
        NumberAnimation
        {
            target: meanLine
            property: "opacity"
            to: 1
            duration: 2000 * idea.animationSpeed
            easing.type: Easing.InOutQuad
        }
        NumberAnimation
        {
            target: meanText
            property: "opacity"
            to: 1
            duration: 2000 * idea.animationSpeed
            easing.type: Easing.InOutQuad
        }
    }

    Rectangle
    {
        id: meanLine
        opacity: 0.0;
        width: 2;
        color: "#de1616"
        height: 210
        y: 20
        x: 25 + 448 * (idea.mean - idea.lowBound)/(idea.upBound - idea.lowBound);

        RectangularGlow
        {
            id: effect
            x: 0
            y: 0
            width: parent.width
            height: parent.height

            glowRadius: 6
            spread: 0.4
            color: "#de1616"
            opacity: 0.1
        }

        Rectangle
        {
            id: meanText

            width: meanWords.width + 4
            height: meanWords.height + 4
            x: -width/ 2
            y: -17

            color: "black"
            border.width: 1
            border.color: "#1a1a1a"
            opacity: 0.0;
            Text
            {
                id: meanWords
                opacity: parent.opacity;
                anchors.centerIn: parent
                font.pointSize: 10
                font.bold: true

                text: "Mean: " + idea.mean.toPrecision(3);
                color: "#de1616"
            }
        }
    }

    Text
    {
        y: 235
        x: 20 + 5 - width/2
        text: idea.lowBound.toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 235
        x: 82.5 + 50 + 5 -width/2
        text: (0.75*idea.lowBound + 0.25*idea.upBound).toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 235
        x: 195 + 50 + 5 -width/2
        text: (0.5*idea.lowBound + 0.5*idea.upBound).toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 235
        x: 307.5 + 50 + 5 -width/2
        text: (0.25*idea.lowBound + 0.75*idea.upBound).toPrecision(3)
        color: "#80b3ff"
    }

    Text
    {
        y: 235
        x: 420 + 50 + 5  -width/2
        text: idea.upBound.toPrecision(3)
        color: "#80b3ff"
    }

    Rectangle
    {
        id: controlRowBackground
        anchors.fill: controlsRow
        anchors.rightMargin: -5
        anchors.leftMargin: -5
        radius: 3
        color: "#342b3b"
    }

    Row
    {
        id: controlsRow
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        spacing: 0

        Rectangle
        {
            anchors.verticalCenter: parent.verticalCenter
            height: 28
            width: 130
            radius: 2
            color: "#1e7d1e"
            Text
            {
                id: lowText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 4
                text: "Low: "

            }
            TextInput
            {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: lowText.right
                anchors.leftMargin: 3
                text: idea.lowBound.toPrecision(3);
                validator: DoubleValidator{}
                selectByMouse: true
                onEditingFinished:
                {
                    idea.setLowBound(text);
                }
            }
        }

        Rectangle
        {
            height: 10
            width: 10
            opacity: 0.0
        }

        Button
        {
            height: 40
            text: "Replay"
            onClicked:
            {
                idea.replay();
            }
        }

        CheckBox
        {
            text: ""
            height: 40
            checked: idea.autoFit;
            onCheckedChanged:
            {
                idea.setAutoFit(checked);
            }
        }
        Text
        {
            anchors.verticalCenter: parent.verticalCenter
            text: "AutoFit";
            color: "#cfbdca"
        }
    }
    Rectangle
    {
        anchors.verticalCenter: controlsRow.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 5
        height: 28
        width: 130
        radius: 2
        color: "#1e7d1e"
        Text
        {
            id: highText
            font.pointSize: 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 4
            text: "High:"
        }

        TextInput
        {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: highText.right
            anchors.leftMargin: 3
            text: idea.upBound.toPrecision(3);
            validator: DoubleValidator{}
            selectByMouse: true
            onEditingFinished:
            {
                idea.setUpBound(text);
            }
        }

    }
}
