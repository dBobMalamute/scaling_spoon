import QtQuick 2.12
import QtQuick.Controls 2.12
import org.test.test 1.0

Rectangle
{
    id: root
    width: 500
    height: 300
    radius: 10
    color: "#deaa87"
    TextArea
    {
        id: textArea

        anchors.fill: parent
        anchors.rightMargin: 7
        anchors.leftMargin: 7
        color: "black"
        textFormat: Qt.RichText
        placeholderText: "Right click green border to make the font bold/italic/colored etc."
        placeholderTextColor: "#555555"
        text: idea.text;
        wrapMode: TextEdit.WordWrap
        font.pointSize: 12
        selectByMouse: true
    }

    DocumentHandler
    {
        id: document
        target: textArea
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        textColor: colorDialog.color
        Component.onCompleted: document.fileUrl = "qrc:/example.html"
        onFontSizeChanged:
        {
            fontSizeSpinBox.valueGuard = false
            fontSizeSpinBox.value = document.fontSize
            fontSizeSpinBox.valueGuard = true
        }
        onFontFamilyChanged:
        {
            var index = Qt.fontFamilies().indexOf(document.fontFamily)
            if (index == -1)
            {
                fontFamilyComboBox.currentIndex = 0
                fontFamilyComboBox.special = true
            }
            else
            {
                fontFamilyComboBox.currentIndex = index
                fontFamilyComboBox.special = false
            }
        }
        onError:
        {
            errorDialog.text = message
            errorDialog.visible = true
        }
    }
}


