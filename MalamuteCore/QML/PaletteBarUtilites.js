var itemComponent = null;
var draggedItem = null;
var startingMouse;
var posnInWindow;

function startDrag(mouse, iconSource)
{
    posnInWindow = mapToItem(palleteBar,mouse.x - 30, mouse.y + 30)

    startingMouse = {x: mouse.x, y: mouse.y}
    loadComponent(iconSource);
}

function loadComponent(iconSource)
{
    if(itemComponent !== null)
    {
        createItem(iconSource);
        return;
    }

    itemComponent = Qt.createComponent("DragCreationItem.qml")
    createItem(iconSource);
}

function createItem(iconSource)
{
    draggedItem = itemComponent.createObject(applicationWindow, {"x": posnInWindow.x, "y": posnInWindow.y, "iconSource": iconSource});
}

function continueDrag(mouse)
{
    if(draggedItem === null)
        return;

    draggedItem.x = mouse.x + posnInWindow.x - startingMouse.x;
    draggedItem.y = mouse.y + posnInWindow.y - startingMouse.y;
}

function endDrag(mouse)
{
    if(draggedItem === null)
    {
        return;
    }
    draggedItem.destroy();
    draggedItem = null;
}
