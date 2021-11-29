import com.malamute.core 1.0
import QtGraphicalEffects 1.12
import QtQuick 2.12
import QtQuick.Controls 2.12

MalamuteCasing
{
    id: root
    width: casingSize.width + 1.4 * 15 + 10;
    height: casingSize.height + 0.4 * 15;

    Rectangle
    {
        id: frameRect
        x: 0.7 * 15; y: 0.2 * 15;
        width: casingSize.width;
        height: casingSize.height
        radius: CasingStyle.frameRadius();
        border.color: selected ? CasingStyle.borderSelectedColor() : CasingStyle.borderColor()
        border.width: CasingStyle.bordersWidth();

        gradient: Gradient
        {
            GradientStop{id: gradientTop; position: 0.25; color: CasingStyle.gradientTopColor(predefinedColor)}
            GradientStop{id: gradientBottom; position: 0.95; color: CasingStyle.gradientBottomColor(predefinedColor)}
        }
        Connections
        {
            //Note. Do not put the connections in a Gradient type. It breaks for some reason.
            target: root
            function onPredefinedColorChanged(i)
            {
                gradientTop.color = CasingStyle.gradientTopColor(i);
                gradientBottom.color = CasingStyle.gradientBottomColor(i);
            }
        }

        RectangularGlow
        {
            id: effect
            x: CasingStyle.glowRadius()
            y: CasingStyle.glowRadius()
            width: frameRect.width
            height: frameRect.height

            glowRadius: CasingStyle.glowRadius()
            spread: CasingStyle.glowSpread()
            color: CasingStyle.glowColor()
            cornerRadius: frameRect.radius + CasingStyle.glowRadius()
            z: frameRect.z - 1
        }

        Column
        {
            id: inPortsColumn
            y: plugBoxTop
            x: -0.5*15 + frameRect.border.width / 2;
            width: 15;

            spacing:  CasingStyle.portSpacing();

            Repeater
            {
                model: inPlugLabels.length
                Rectangle
                {
                    radius: CasingStyle.portSize();
                    width: CasingStyle.portSize();
                    height: CasingStyle.portSize();
                    border.color: frameRect.border.color
                    border.width: CasingStyle.portBorderWidth();
                    color: inPlugColor(index)
                }
            }
        }

        Column
        {
            id: outPortsColumn
            y: plugBoxTop
            x: frameRect.width - 0.5*15 - frameRect.border.width / 2;
            width: 15;

            spacing: CasingStyle.portSpacing();

            Repeater
            {
                model: outPlugLabels.length
                Rectangle
                {
                    id: outPortRect
                    radius: CasingStyle.portSize();
                    width: CasingStyle.portSize();
                    height: CasingStyle.portSize();
                    border.color: frameRect.border.color
                    border.width: CasingStyle.portBorderWidth();
                    color: outPlugColor(index)

                    RectangularGlow
                    {
                        x: CasingStyle.glowRadius()
                        y: CasingStyle.glowRadius()
                        width: outPortRect.width
                        height: outPortRect.height

                        glowRadius: CasingStyle.glowRadius()
                        spread: CasingStyle.glowSpread()
                        color: CasingStyle.glowColor()
                        cornerRadius: outPortRect.radius + CasingStyle.glowRadius()
                        z: outPortRect.z - 1
                    }
                }
            }
        }
    }

    Text
    {
        id: name
        anchors.top: frameRect.top;
        anchors.topMargin: 5 + frameRect.border.width
        anchors.horizontalCenter: frameRect.horizontalCenter;
        width: nameSize.width
        height: nameSize.height
        color: CasingStyle.nameColor()

        font: CasingStyle.nameFont()
        horizontalAlignment: Text.AlignHCenter
        text: ideaName
    }

    Column
    {
        id: inPortLabelsColumn
        x: 0.7 * 15 + inPortsColumn.x + inPortsColumn.width + 5;
        y: 0.2 * 15 + plugBoxTop - 1;
        width: inLabelWidth

        spacing: CasingStyle.portSpacing();

        Repeater
        {
            model: inPlugLabels
            Text
            {
                height: CasingStyle.portSize();
                font: CasingStyle.portLabelsFont();
                color: CasingStyle.portLabelsColor();
                text: modelData
            }
        }
    }

    Column
    {
        id: outPortLabelsColumn
        y: inPortLabelsColumn.y
        x: 0.7 * 15 + outPortsColumn.x - outLabelWidth - 5;
        width: outLabelWidth

        spacing: CasingStyle.portSpacing();

        Repeater
        {
            model: outPlugLabels
            Text
            {
                height: CasingStyle.portSize();
                font: CasingStyle.portLabelsFont();
                color: CasingStyle.portLabelsColor();
                text: modelData
            }
        }
    }

    Rectangle
    {
        color: validationColor
        radius: CasingStyle.frameRadius();

        y: validationBoxTop
        anchors.left: frameRect.left;
        width: frameRect.width
        height: validationBoxSize.height === 0 ? 0 : validationBoxSize.height + 2*5 + 2*border.width

        border.color: frameRect.border.color;
        border.width: frameRect.border.width;
        Text
        {
            id: validatationMessageDisplay
            color: CasingStyle.validationTextColor();
            font: CasingStyle.validationTextFont();
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            text: validationMessage
        }
    }

    Rectangle
    {
        id: minimizerButton
        visible: minimizable
        width: CasingStyle.buttonSize()
        height: CasingStyle.buttonSize()
        color: "transparent"
        radius: CasingStyle.buttonSize()
        anchors.top: frameRect.top
        anchors.topMargin: -3
        anchors.right: frameRect.right
        anchors.rightMargin: 10

        MouseArea
        {
            anchors.fill: parent
            anchors.margins: -1
            hoverEnabled: true
            onClicked:
            {
                minimizeButtonClicked();
            }
            onEntered:
            {
                minimizerButton.color = Qt.lighter("#845a35",1.5)
            }
            onExited:
            {
                minimizerButton.color = "transparent"
            }
        }
    }

    Rectangle
    {
        visible: minimizable
        width: 6
        height: 2
        color: Qt.darker("#845a35",1.5)
        anchors.centerIn: minimizerButton
    }

    Rectangle
    {
        id: extraButton
        visible: hasExtraQML
        width: CasingStyle.buttonSize()
        height: CasingStyle.buttonSize()
        color: "transparent"
        radius: CasingStyle.buttonSize()
        anchors.top: frameRect.top
        anchors.topMargin: 3
        anchors.right: frameRect.right
        anchors.rightMargin: -1

        MouseArea
        {
            anchors.fill: parent
            anchors.margins: -1
            hoverEnabled: true
            onClicked:
            {
                extraButtonClicked();
            }
            onEntered:
            {
                extraButton.color = Qt.lighter(CasingStyle.buttonColor(),1.5)
            }
            onExited:
            {
                extraButton.color = "transparent"
            }
        }
    }

    Rectangle
    {
        visible: hasExtraQML
        width: 4
        height: 2
        color: Qt.darker(CasingStyle.buttonColor(),1.5)
        anchors.centerIn: extraButton
    }
    Rectangle
    {
        visible: hasExtraQML
        width: 2
        height: 4
        color: Qt.darker(CasingStyle.buttonColor(),1.5)
        anchors.centerIn: extraButton
    }

    Rectangle
    {
        id: resizer;
        visible: resizeable
        x: frameRect.x + frameRect.width - 5 - 17;
        y: frameRect.y + frameRect.height - 5- 17;
        z: frameRect.z + 1;
        width: CasingStyle.resizerSize()
        height: CasingStyle.resizerSize()
        radius: CasingStyle.resizerSize()
        border.width: 2;
        color: CasingStyle.resizerColor()
        border.color: CasingStyle.resizerBorderColor()
    }
}
