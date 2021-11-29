import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import com.malamute.core 1.0

MalamutePostIt
{
    id: postItRoot
    width: Math.max(300,title.implicitWidth + 60)
    height: title.implicitHeight + shortMessage.implicitHeight + longMessage.implicitHeight + 30;

    RectangularGlow
    {
        id: effect
        x: 6
        y: 6
        width: postItRoot.width
        height: postItRoot.height

        glowRadius: PostItStyle.glowRadius()
        spread: PostItStyle.glowSpread()
        color: PostItStyle.glowColor()
        cornerRadius: glowRadius
        z: postItRoot.z - 1
    }

    Rectangle
    {
        anchors.fill: parent
        gradient: Gradient
        {
            GradientStop{position: 0.1; color: PostItStyle.gradientTop()}
            GradientStop{position: 0.9; color: PostItStyle.gradientBottom()}
        }
        border.width: PostItStyle.borderWidth()
        border.color: postItRoot.selected ? PostItStyle.borderColorSelected() : PostItStyle.borderColor()
    }

    TextEdit
    {
        id: title

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.margins: 6
        color: PostItStyle.titleColor();
        font: PostItStyle.titleFont();
        selectByMouse: true
        text: postItRoot.title;
        readOnly: true
    }

    TextEdit
    {
        id: shortMessage
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: title.bottom
        anchors.margins: 6
        color: PostItStyle.shortMessageColor();
        font: PostItStyle.shortMessageFont();
        selectByMouse: true
        text: postItRoot.shortMessage;
        readOnly: true;
    }

    TextEdit
    {
        id: longMessage
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: shortMessage.bottom
        anchors.margins: 6
        wrapMode: TextEdit.Wrap
        color: PostItStyle.longMessageColor();
        font: PostItStyle.longMessageFont();
        selectByMouse: true
        text: postItRoot.longMessage;
        readOnly: true;
    }

    Image
    {
        id: closeButton
        source: "qrc:/Images/CloseIcon.png"
        width: 15
        height: 15
        x: postItRoot.width - width - 1;
        y: 1;

        MouseArea
        {
            anchors.fill: parent
            anchors.margins: -5
            onClicked:
            {
                postItRoot.deletePostIt();
            }
        }
    }
}
