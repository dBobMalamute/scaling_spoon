import QtQuick 2.12
import QtQuick.Controls 2.12
import com.malamute.controlsStyles 1.0

Rectangle
{
    width: 160
    height: 180
    radius: ControlsStyle.radius()
    border.color: ControlsStyle.borderColor()
    border.width: ControlsStyle.borderWidth()

    gradient: Gradient
    {
        GradientStop{ position: 0.05; color: ControlsStyle.gradientTop()}
        GradientStop{position: 0.5; color: ControlsStyle.gradientMid()}
        GradientStop{ position: 0.95; color: ControlsStyle.gradientBottom()}
    }

    Column
    {
        spacing: 2
        x: 10
        y: 4
        Row
        {
            CheckBox
            {
                checked: idea.autoFitGrid;
                onCheckedChanged:
                {
                    idea.setAutoFitGrid(checked);
                }
            }
            Text
            {
                anchors.verticalCenter: parent.verticalCenter
                text: "Fit Axis";
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
            }
        }

        Row
        {
            spacing: 5
            Text
            {
                text: "x Min:";
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
            }
            TextInput
            {
                text: idea.gridBounds[0].toPrecision(3);
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
                validator: DoubleValidator{}
                selectByMouse: true
                onEditingFinished: idea.setGridBound(text, 0)
            }
        }
        Row
        {
            spacing: 5
            Text
            {
                text: "x Max:";
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
            }
            TextInput
            {
                text: idea.gridBounds[1].toPrecision(3);
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
                validator: DoubleValidator{}
                selectByMouse: true
                onEditingFinished: idea.setGridBound(text, 1)
            }
        }
        Row
        {
            spacing: 5
            Text
            {
                text: "y Min:";
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
            }
            TextInput
            {
                text: idea.gridBounds[2].toPrecision(3);
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
                validator: DoubleValidator{}
                selectByMouse: true
                onEditingFinished: idea.setGridBound(text, 2)
            }
        }
        Row
        {
            spacing: 5
            Text
            {
                text: "y Max:";
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
            }
            TextInput
            {
                text: idea.gridBounds[3].toPrecision(3);
                font: ControlsStyle.textFont();
                color: ControlsStyle.textColor();
                validator: DoubleValidator{}
                selectByMouse: true
                onEditingFinished: idea.setGridBound(text, 3)
            }
        }
    }
}
