/**
  ToolbarLayout - distributes the
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


Item {
    // set to true if content can be resized to cover toolbar area
    property bool resizeToolsToFit: true
    // specify whether to center buttons in the dock; not used if resuzeToolsToFit is set to true
    property bool centerLayout: false
    // spacing in between the elements; used if elements are not resized
    property int spacing: THEME.sizes.spacingSmall
    // specifies teh current toolbar
    property Item toolBar

    id: root
    anchors.fill: parent
    anchors.margins: THEME.sizes.spacingSmall
    onChildrenChanged: privates.layout(children)
    onWidthChanged: privates.layout(children)
    onHeightChanged: privates.layout(children)
    Component.onCompleted: privates.layout(children)

    QtObject {
        id: privates
        function layout(itemList)
        {
            var i, itemCount
            var kw = 0, sp = spacing;
            var dx = 0
            var items = new Array()

            // count visual items
            for (i = 0; i < itemList.length; i++) {
                if (itemList[i].objectName !== "NonVisualElement")
                    items.push(itemList[i])
            }
            itemCount = items.length

            if (resizeToolsToFit) {
                /// all kids have the same width
                kw = width/itemCount - (itemCount - 1)*sp
            } else if (centerLayout) {
                for (i = 0; i < itemCount; i++) {
                    kw += items[i].width + sp
                }
                kw -= sp
                dx = (width - kw) / 2
            } else {
                // calculate spacing so that kids are distributed evenly
                kw = width
                for (i = 0; i < itemCount; i++) {
                    kw -= (items[i].width + sp)
                }
                sp = kw/(itemCount - 1)
            }

            for (i = 0; i < itemCount; i++) {
                var child = items[i]
                //center vertically
                child.anchors.verticalCenter = verticalCenter
                if (resizeToolsToFit)
                    child.width = kw
                child.height = height
                child.x = dx
                dx += sp + child.width
            }
        }
    }
}

/*
CheckGroup {
    // toolbar item holding the layout
    property Dock toolbar
    // alignment
    property int alignment: Qt.AlignJustify
    default property alias content: body.data

    id: group
    anchors.fill: parent
    autoId: true
    resizeChildrenToGroup: false
    Row {
        id: body
        property int marginOffset: 0
        property int defSpacing: THEME.sizes.spacingSmall
        anchors {
            fill: parent
            leftMargin: 1 + marginOffset
            topMargin: 1
            rightMargin: 1 + marginOffset
            bottomMargin: 1
        }
        //width: parent.width
        spacing: layoutChildren()
        function layoutChildren()
        {
            if (group.resizeChildrenToGroup) {
                console.debug("dock resizes its children")
                return defSpacing
            }

            // todo: rework this a bit...
            var icount = body.children.length
            var fw = group.width - 2
            switch (group.alignment) {
            case Qt.AlignHCenter:
                var cw = children[0].width * icount + (icount - 1) * defSpacing
                marginOffset = (fw - cw) / 2
                return defSpacing
            case Qt.AlignJustify:
                marginOffset = 0
                return (fw - icount * children[0].width) / (icount - 1) - 1
            default:
                return defSpacing
            }
        }
    }
}
*/
