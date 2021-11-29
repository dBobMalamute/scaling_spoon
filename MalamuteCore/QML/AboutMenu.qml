import QtQuick 2.12

Rectangle
{
    width: 450
    height: 400
    color: "#cccccc"
    border.color: "#303030"
    border.width: 4


    Text
    {
        anchors.centerIn: parent
        width: 430
        height: 380
        horizontalAlignment: Text.AlignHCenter

        text: "The software is written in qt c++, qt markup language (qml), Javascript.
Compiled into Webassembly(Wasm) by the Emscripten compiler.
Artwork was made in inkscape.

The source code is on github.

Roughly half of the core is an extensively modified version of
nodeeditor. The appropriate copyright information is listed on
the respective files (Source Code) and in the legal section."

    }
}

