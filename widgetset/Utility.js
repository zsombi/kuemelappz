//.pragma library

/**

  */
var pgStack = [];
function getPgStack()
{
    return pgStack
}

function rootObject(obj)
{
    if (!obj.parent)
        return obj
    else
        return rootObject(obj.parent)
}

function namedParent(obj, name)
{
    //console.debug("obj: " + obj)
    if (obj && (obj.objectName == name))
        return obj
    if (obj.parent)
        return namedParent(obj.parent, name);
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
