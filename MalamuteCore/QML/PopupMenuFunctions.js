var comp = null;

var object = null;

function createAboutMenu()
{
    comp = Qt.createComponent("AboutMenu.qml");
    object = comp.createObject(corkboard,{y: -1.0*corkboard.y, x: -1.0*corkboard.x});
}

function createLegalMenu()
{
    comp = Qt.createComponent("LegalMenu.qml");
    object = comp.createObject(corkboard,{y: -1.0*corkboard.y, x: -1.0*corkboard.x});
}
