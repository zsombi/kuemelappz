// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/*
  Dock item to hold styled items; base for Toolbar, PageTab, etc
  */

StyledItem {

    // active tool ID
    property alias activeToolId: group.activeId
    // assign ID to tool buttons automatically
    property alias autoId: group.autoId
    // default property to hold content
    default property alias content: body.data
    // resize content to fill dock entirely
    property alias resizeChildrenToDock: group.resizeChildrenToGroup
    // how to align tool buttons in the dock
    property variant alignment: Qt.AlignJustify

    styleName: "ThemeDockStyle"
    id: dock
    width: parent.width
    height: THEME.sizes.dockHeight

    Background {
        id: frame
        anchors.fill: parent
        styleName: dock.style.frameStyle
        styleType: dock.styleType
    }

    CheckGroup {
        id: group
        anchors.fill: parent
        autoId: true
        resizeChildrenToGroup: false
        Row {
            id: body
            property int marginOffset: 0
            property int defSpacing: dock.style.buttonSpacing
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
                var fw = styleFrame.width - 2
                switch (dock.alignment) {
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
}
