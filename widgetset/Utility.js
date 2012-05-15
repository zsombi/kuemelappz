//.pragma library

/**

  */
var pgStack = [];
function getPgStack()
{
    return pgStack
}

function rootObject(item)
{
    var obj = item
    while (obj.parent)
        obj = obj.parent
    return obj
}

function namedParent(item, name)
{
    var obj = item
    while (obj) {
        //console.debug("obj: " + obj)
        if (obj && (obj.objectName == name))
            return obj
        obj = obj.parent
    }
    return null
}

function flickableParent(item)
{
    var obj = item
    while (obj) {
        if ((obj.flicking !== undefined) && (obj.flickableDirection !== undefined))
            return obj
        obj = obj.parent
    }
    return null
}

function pageLayout(item)
{
    var obj = item
    while (obj) {
        if ((obj.hideHeaderWhenEmpty !== undefined) && (obj.hideToolbarWhenEmpty !== undefined))
            return obj
        obj = obj.parent
    }
    return null
}

function screenX(obj)
{
    if (obj && obj.parent)
        return obj.x + screenX(obj.parent)
    return obj.x;
}

function screenY(obj)
{
    if (obj && obj.parent)
        return obj.y + screenY(obj.parent)
    return obj.y;
}

/**
  returns whether system's input panel should be used or not
  */
function usePlatformInput()
{
    return rootObject(widgetSet.applicationBody).usePlatformInput
}

/**
  opens the input panel as defined

  */
function openInputPanel(focusControl, flags, layout)
{
    if (widgetSet.mobilePlatform) {
        if (usePlatformInput())
            focusControl.openSoftwareInputPanel()
        else
            widgetSet.inputPanel.openInput(focusControl, flags, layout)
    }
}

/**
  closes the input panel
  */
function closeInputPanel(focusControl)
{
    if (widgetSet.mobilePlatform) {
        if (usePlatformInput())
            focusControl.closeSoftwareInputPanel()
        else
            widgetSet.inputPanel.closeInput()
    }
}
