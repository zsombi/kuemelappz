.pragma library

function namedParent(obj, name)
{
    console.debug("obj: " + obj)
    if (obj && (obj.objectName == name))
        return obj
    if (obj.parent)
        return namedParent(obj.parent, name);
    return null
}
