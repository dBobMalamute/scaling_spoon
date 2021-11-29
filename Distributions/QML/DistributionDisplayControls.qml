import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle
{
    id: root

    width: 400
    height: 300
    color: "#9f4caf"
    border.color: "#5e2d69"
    border.width: 4
    radius: 20
    z: 1000

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        RowLayout
        {
            Label
            {
                text: "Title: "
                font.pointSize: 12
                font.bold: true
            }

            TextInput
            {
                text: idea.title
                font.pointSize: 12
                font.bold: idea.titleBold
                selectByMouse: true
                onEditingFinished:
                {
                    idea.setTitle(text);
                }
                onFocusChanged:
                {
                    if(focus === true & idea.title === "Right click to set Title")
                        idea.setTitle("");
                }
            }
        }
        RowLayout
        {
            Label
            {
                text: "Bold"
                font.pointSize: 12
                font.bold: true
            }
            Switch
            {
                checked: idea.titleBold
                onClicked: idea.setTitleBold(checked);
            }
            Label
            {
                text: "Size"
                font.pointSize: 12
                font.bold: true
            }
            TextInput
            {
                text: idea.titleFontSize
                font.pointSize: 12
                selectByMouse: true
                validator: IntValidator{bottom: 1; top: 100}
                onEditingFinished:
                {
                    idea.setTitleFontSize(text);
                }
            }
        }
        RowLayout
        {
            Label
            {
                text: "Y Axis Label:"
                font.pointSize: 12
                font.bold: true
            }
            TextInput
            {
                text: idea.yAxisLabel
                font.pointSize: 12
                selectByMouse: true
                width: 100
                onTextChanged:
                {
                    idea.setYAxisLabel(text);
                }
            }
            Label
            {
                text: "yMin"
                font.pointSize: 12
                font.bold: true
            }
            TextInput
            {
                text: idea.yMin
                font.pointSize: 12
                font.bold: true
                selectByMouse: true
                validator: DoubleValidator{}                
                onEditingFinished:
                {
                    if(validator.Acceptable)
                    {
                        idea.setYMin(text);
                    }
                }
            }
            Label
            {
                text: "yMax"
                font.pointSize: 12
                font.bold: true
            }
            TextInput
            {
                text: idea.yMax
                font.pointSize: 12
                font.bold: true
                selectByMouse: true
                validator: DoubleValidator{}
                onEditingFinished:
                {
                    idea.setYMax(text)
                }
            }
        }

        RowLayout
        {
            Label
            {
                text: "X Axis Label:"
                font.pointSize: 12
                font.bold: true
            }
            TextInput
            {
                text: idea.xAxisLabel
                font.pointSize: 12
                selectByMouse: true
                width: 100
                onEditingFinished:
                {
                    idea.setXAxisLabel(text)
                }
            }
            Label
            {
                text: "xMin"
                font.pointSize: 12
                font.bold: true
            }
            TextInput
            {
                text: idea.xMin
                font.pointSize: 12
                font.bold: true
                selectByMouse: true
                validator: DoubleValidator{}
                onEditingFinished:
                {
                    idea.setXMin(text)
                }
            }
            Label
            {
                text: "xMax"
                font.pointSize: 12
                font.bold: true
            }
            TextInput
            {
                text: idea.xMax
                font.pointSize: 12
                font.bold: true
                selectByMouse: true
                validator: DoubleValidator{}
                onEditingFinished:
                {
                    idea.setXMax(text);
                }
            }
        }
        RowLayout
        {
            CheckBox
            {
                checked: idea.autoFitXAxis
                text: "Auto fit x."
                onCheckedChanged:
                {
                    idea.setAutoFitXAxis(checked)
                }
            }
            CheckBox
            {
                checked: idea.autoFitYAxis
                text: "Auto fit y"
                onCheckedChanged:
                {
                    idea.setAutoFitYAxis(checked)
                }
            }
        }
    }
}
