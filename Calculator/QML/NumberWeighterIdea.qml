import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Item
{
    width:  Math.max(topNumInput.width,botNumInput.width) + slider.width
    height: topNumInput.height + botNumInput.height + 2;

    TextInput
    {
        id: topNumInput
        font.bold: true
        font.pointSize: 12;
        color: "white"
        selectByMouse: true
        validator: DoubleValidator{top: 1.0; bottom: 0.0}
        onEditingFinished:
        {
            idea.setWeight(text);
        }

        Connections
        {
            target: idea
            function onWeightChanged()
            {
                topNumInput.text = idea.topWeightString();
            }
        }
        Component.onCompleted:
        {
            text = idea.topWeightString();
        }
    }
    TextInput
    {
        id: botNumInput
        anchors.top: topNumInput.bottom;
        anchors.topMargin: 4
        anchors.bottomMargin: 4
        font.bold: true
        font.pointSize: 12
        color: "white"
        selectByMouse: true
        validator: DoubleValidator{top: 1.0; bottom: 0.0}

        onEditingFinished:
        {
            idea.setWeight(1.0 - text);
        }

        Connections
        {
            target: idea
            function onWeightChanged()
            {
                botNumInput.text = idea.botWeightString();
            }
        }
        Component.onCompleted:
        {
            text = idea.botWeightString();
        }

    }

    Slider
    {
        id: slider
        width: 40
        height: 60; y: -5
        x: 40
        orientation: Qt.Vertical
        from: 0.0; to: 1.0;
        value: idea.weight
        onMoved:
        {
            idea.setWeight(value)
        }

        handle: Rectangle
        {

        }

        Connections
        {
            target: idea
            function onWeightChanged()
            {

            }
        }
    }
}



