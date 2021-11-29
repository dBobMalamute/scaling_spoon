var borderWidth
var xSpacing;
var xBoxNum;
var ySpacing;

function setupForBoxes(numBoxes)
{
    var boxComponent;
    var boxObject;

    var wholeBoxes = Math.floor(numBoxes);

    if(wholeBoxes <= 25)
    {
        borderWidth = 2;
        xSpacing = 80;
        xBoxNum = 5;
        ySpacing = 60;
    }
    else if(wholeBoxes <= 100)
    {
        borderWidth = 2;
        xSpacing = 40;
        xBoxNum = 10;
        ySpacing = 30;
    }
    else if (wholeBoxes <= 400)
    {
        borderWidth = 2;
        xSpacing = 20;
        xBoxNum = 20;
        ySpacing = 15;
    }
    else if (wholeBoxes <= 2500)
    {
        borderWidth = 1;
        xSpacing = 8;
        xBoxNum = 50;
        ySpacing = 6;
    }
    else
    {
        borderWidth = 1;
        xSpacing = 4;
        xBoxNum = 100;
        ySpacing = 3;
    }


        boxComponent = Qt.createComponent("PowerBox.qml");
        boxObject = boxComponent.createObject(animationArea, {
                                                  borderWidth: borderWidth,
                                                  x: 0, y: 0,
                                                  width: xSpacing, height: ySpacing})
}

function clearBoxes()
{
    for(var i = 0; i < animationArea.children.length; i++)
    {
        animationArea.children[i].destroy();
    }
    removeFractionBox();
}

function addBoxes(currentNum)
{
    var boxComponent;
    var boxObject;
    for(var i = animationArea.children.length; i < currentNum; i++)
    {
        boxComponent = Qt.createComponent("PowerBox.qml");
        boxObject = boxComponent.createObject(animationArea, {
                                                  borderWidth: borderWidth,
                                                  x: xSpacing*(i%xBoxNum), y: ySpacing*Math.floor((i/xBoxNum)),
                                                  width: xSpacing, height: ySpacing})
    }
}

function finishAddingBoxes(currentNum)
{
    var boxComponent;
    var boxObject;
    for(var i = animationArea.children.length; i <= currentNum - 1; i++)
    {
        boxComponent = Qt.createComponent("PowerBox.qml");
        boxObject = boxComponent.createObject(animationArea, {
                                                  borderWidth: borderWidth,
                                                  x: xSpacing*(i%xBoxNum), y: ySpacing*Math.floor((i/xBoxNum)),
                                                  width: xSpacing, height: ySpacing})
    }
    boxComponent = Qt.createComponent("PowerBox.qml");
    boxObject = boxComponent.createObject(fractionAnimationArea, {borderWidth: borderWidth,
                                              x: xSpacing*(Math.floor(currentNum) % xBoxNum),
                                              y: ySpacing*(Math.floor(currentNum/xBoxNum)),
                                              width: xSpacing * (currentNum % 1), height: ySpacing})
}

function addBoxesWithFraction(currentNum)
{
    removeFractionBox();

    var boxComponent;
    var boxObject;
    for(var i = animationArea.children.length; i <= currentNum - 1; i++)
    {
        boxComponent = Qt.createComponent("PowerBox.qml");
        boxObject = boxComponent.createObject(animationArea, {
                                                  borderWidth: borderWidth,
                                                  x: xSpacing*(i%xBoxNum), y: ySpacing*Math.floor((i/xBoxNum)),
                                                  width: xSpacing, height: ySpacing})
    }
    boxComponent = Qt.createComponent("PowerBox.qml");
    boxObject = boxComponent.createObject(fractionAnimationArea, {borderWidth: borderWidth,
                                              x: xSpacing*(Math.floor(currentNum) % xBoxNum),
                                              y: ySpacing*(Math.floor(currentNum/xBoxNum)),
                                              width: xSpacing * (currentNum % 1), height: ySpacing})
}

function removeFractionBox()
{
    for(var i = 0; i < fractionAnimationArea.children.length; i++)
    {
        fractionAnimationArea.children[i].destroy();
    }
}
