import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Shapes 1.12

import com.distributions.dists 1.0

Item
{
    id: root
    width: 530
    height: 424

    DistributionDisplay
    {

    }

    Item
    {
        id: dotsArea
        x: 55
        y: 10
        visible: idea.valid;
        width: 450
        height: 300

        property int numDotsShown: 0;

    }

    Connections
    {
        target: idea
        function onStartAnimation()
        {
            if(idea.totalSamples > dotsArea.numDotsShown)
                addDotAnimation.start();
        }
        function onResetAnimation()
        {
            addDotAnimation.stop();

            for(var j = 0; j < dotsArea.children.length; j++)
            {
                dotsArea.children[j].destroy();
            }
            dotsArea.numDotsShown = 0;
        }
    }

    SequentialAnimation
    {
        id: addDotAnimation;
        running: false;
        loops: Animation.Infinite

        ScriptAction
        {
            script: root.addNextDot();
        }
        PauseAnimation
        {
            duration: 200
        }
    }

    function addNextDot()
    {
        if(!idea.valid || dotsArea.numDotsShown >= idea.totalSamples)
            addDotAnimation.stop();
        else
        {
            var comp = Qt.createComponent("DistributionSamplerDot.qml");
            var obj = comp.createObject(dotsArea,{x: dotsArea.numDotsShown % 2 * 450});
            var pos = idea.getPositionForDot(dotsArea.numDotsShown);
            obj.x = pos.x  - 2;
            obj.y = pos.y;
            dotsArea.numDotsShown++;
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
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5

        spacing: 10
        Button
        {
            anchors.verticalCenter: parent.verticalCenter
            text: "Sample"
            height: 40
            enabled: idea.valid;

            onClicked:
            {
                idea.sampleButtonPressed();
            }
        }
        ComboBox
        {
            id: comboBox
            textRole: "key"
            height: 40
            width: 160
            model: ListModel
            {
                ListElement {key: "One"; value: 1 }
                ListElement {key: "Five"; value: 5 }
                ListElement {key: "Ten"; value: 10 }
                ListElement {key: "One Hundred"; value: 100 }
                ListElement {key: "One Thousand"; value: 1000 }
                ListElement {key: "Ten Thousand"; value: 10000 }

            }
            onActivated:
            {
                switch(index)
                {
                case 0:
                    idea.setSamplingSize(1)
                    break;
                case 1:
                    idea.setSamplingSize(5)
                    break;
                case 2:
                    idea.setSamplingSize(10)
                    break;
                case 3:
                    idea.setSamplingSize(100)
                    break;
                case 4:
                    idea.setSamplingSize(1000)
                    break;
                case 5:
                    idea.setSamplingSize(10000)
                    break;
                default:
                    idea.setSamplingSize(1);
                }
            }
            Component.onCompleted:
            {
                comboBoxConnection.onSamplingSizeChanged(idea.samplingSize)
            }

            Connections
            {
                id: comboBoxConnection
                target: idea
                function onSamplingSizeChanged(n)
                {
                    switch(n)
                    {
                    case 1:
                        comboBox.currentIndex = 0;
                        break;
                    case 5:
                        comboBox.currentIndex = 1;
                        break;
                    case 10:
                        comboBox.currentIndex = 2;
                        break;
                    case 100:
                        comboBox.currentIndex = 3;
                        break;
                    case 1000:
                        comboBox.currentIndex = 4;
                        break;
                    case 10000:
                        comboBox.currentIndex = 5;
                        break;
                    default:
                        comboBox.currentIndex = 0;
                    }
                }
            }
        }

        Button
        {
            height: 40
            anchors.verticalCenter: parent.verticalCenter
            text: "Clear Samples";
            onClicked:
            {
                idea.totalSamplesTextEdited(0);
            }
        }
        FocusScope
        {
            anchors.verticalCenter: parent.verticalCenter
            width: rectangle.width
            height: rectangle.height
            id: scope;

            Rectangle
            {
                id: rectangle
                width: samplesText.implicitWidth + numberText.implicitWidth + 10;
                height: samplesText.implicitHeight + 10
                radius: 3
                color: "#bd9173"
                border.color: "#1a1a1a"
                border.width: 2

                Row
                {
                    id: row
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    Text
                    {
                        id: samplesText
                        text: "Samples: "
                        font.bold: true
                        font.pointSize: 12
                        color: "#1a1a1a"
                    }
                    TextInput
                    {
                        id: numberText
                        text: idea.totalSamples;
                        font.bold: true
                        font.pointSize: 12
                        font.underline: true
                        focus: true
                        color: "#1a1a1a"
                        selectByMouse: true
                        validator: IntValidator{bottom: 0; top: 99999}
                        onEditingFinished:
                        {
                            idea.totalSamplesTextEdited(text);
                        }
                    }
                }
                Image
                {
                    anchors.right: parent.right
                    anchors.rightMargin: -10
                    source: "qrc:/Images/Pencil.png"
                    fillMode: Image.PreserveAspectFit
                    width: 10

                }
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    scope.focus = true;
                }
                onDoubleClicked:
                {
                    numberText.selectAll();
                }
            }
        }
    }
    GridView
    {
        id: samplesListGridView
        anchors.bottom:  controlsRow.top
        anchors.bottomMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: 430
        height: 39
        cellHeight: 16
        cellWidth: 34
        clip: true

        model: idea.samples
        delegate: Rectangle
        {
            required property string modelData

            x: 2
            y: 2
            width: 30
            height: 12
            radius: 2
            color: "#947692"

            Text
            {
                anchors.centerIn: parent
                text: parent.modelData.slice(0,5)
                font.pointSize: 9
            }
        }
    }
    Rectangle
    {
        anchors.top: samplesListGridView.top;
        anchors.topMargin: -4
        anchors.bottom: samplesListGridView.bottom;
        anchors.bottomMargin: -4
        anchors.right: samplesListGridView.right
        anchors.left: parent.left
        radius: 3
        gradient: Gradient
        {
            GradientStop{position: 0.0; color: "#101010"}
            GradientStop{position: 1.0; color: "#303030"}
        }
        z: -1
    }
}
