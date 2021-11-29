import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Shapes 1.12
import QtGraphicalEffects 1.12

import com.distributions.dists 1.0

Item
{
    id: root
    width: 530
    height: 374

    DistributionDisplay
    {
        id: item_distributionDisplay
    }

    Item
    {
        id: rectanglesArea
        visible: idea.valid;
        x: 55
        y: 10
        width: 450;
        height: 300
        clip: true;
    }

    Item
    {
        id: meanBarArea
        visible: idea.valid;
        x: 55
        y: 10
        width: 450
        height: 300
        clip: true;

        Rectangle
        {
            id: meanLine
            opacity: 0.0;
            width: 2;
            color: "#de1616"
            height: 290
            y: 20

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
    }

    Connections
    {
        target: idea;
        function onStartAnimation(continous)
        {
            clearMeanAnimationThings();

            if(continous)
                startContinousAnimation();
            else
                startDiscreteAnimation();
        }

        function clearMeanAnimationThings()
        {
            meanLine.opacity = 0.0;
            meanText.opacity = 0.0;

            //Clear the rectanglesArea.
            for(var i = 0; i < rectanglesArea.children.length; i++)
            {
                rectanglesArea.children[i].destroy();
            }
        }

        function startContinousAnimation()
        {
            const meanPos = 450 * (idea.mean - distributionDisplay.xLow) /
                          (distributionDisplay.xHigh - distributionDisplay.xLow)

            meanLine.x = meanPos - 1;
            var comp;
            var obj;
            var rect;
            var lineDistance;
            for(var i = 0; i < idea.numDots; i++)
            {
                rect = idea.getRectangleParams(i);
                lineDistance = meanPos - (rect.x + rect.width / 2.0)
                comp = Qt.createComponent("DistributionMeanRectangle.qml");
                obj = comp.createObject(rectanglesArea,
                                        {x: rect.x, y: rect.y, width: rect.width, height: rect.height,
                                        i: i, sections: idea.numDots, lineDistance: lineDistance});

            }
            meanLineAnimation.start();
        }

        function startDiscreteAnimation()
        {
            const meanPos = 450 * (idea.mean - distributionDisplay.xLow) /
                          (distributionDisplay.xHigh - distributionDisplay.xLow)
            meanLine.x = meanPos - 1;

            var comp;
            var obj;
            var point;
            var lineDistance;
            for(var i = 0; i < idea.getNumDiscretePoints(); i++)
            {
                point = idea.getPointParams(i);
                lineDistance = meanPos - point.x;
                comp = Qt.createComponent("DistributionMeanWeightedBar.qml");
                obj = comp.createObject(rectanglesArea,
                                        {x: point.x - 1, y: 300 - point.y, height: point.y,
                                        lineDistance: lineDistance})
            }
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
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 0

        Button
        {
            height: 40
            text: "Replay"
            onClicked:
            {
                idea.replayButtonClicked();
            }
        }

        Rectangle
        {
            height: 10
            width: 10
            opacity: 0.0
        }

        Rectangle
        {
            anchors.verticalCenter: controlsRow.verticalCenter
            width: 150
            height: 28
            radius: 2;
            color: "#602020"
            Text
            {
                anchors.verticalCenter: parent.verticalCenter
                x: 10
                color: "#f9f9f9"
                font.pointSize: 14
                text: "Sections:"
            }
            TextInput
            {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                selectByMouse: true;
                color: "#f9f9f9"
                font.bold: true;
                font.pointSize: 14
                validator: IntValidator{bottom: 2; top: 999}
                onEditingFinished:
                {
                    idea.setNumDots(text);
                }
                Component.onCompleted:
                {
                    text = idea.numDots;
                }
            }
        }
    }
}
