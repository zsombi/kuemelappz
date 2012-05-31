// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/*
  Dock item to hold styled items; base for Toolbar, PageTab, etc
  */

Background {
    id: dock
    // current layout object
    property Item dockLayout: null

    // function to set the layout; it can replace the last layout or
    // add a new one to the stack
    function setLayout(layout, replace)
    {
        privates.pushLayout(layout, replace)

        dockLayout = layout
        if (dockLayout) {
            dockLayout.parent = dock
            dockLayout.toolBar = dock
            dockLayout.visible = true
        }
    }
    //removes the last layout from the stack and sets the active one
    function popLayout()
    {
        dockLayout = privates.popLayout()
        dockLayout.visible = true
    }

    QtObject {
        id: privates
        property variant parentStack
        property variant toolStack

        function pushLayout(layout, replace)
        {
            if (!toolStack) {
                toolStack = new Array()
                parentStack = new Array()
                toolStack.push(layout)
                parentStack.push(layout.parent)
            } else if (replace && (toolStack.length > 0)){
                //replace
                //reparent previous layout
                dockLayout.parent = parentStack[parentStack.length - 1]
                dockLayout.visible = false
                toolStack[toolStack.length - 1] = layout
                parentStack[parentStack.length - 1] = layout.parent
            } else {
                // push next in
                // disable previous one
                if (dockLayout)
                    dockLayout.visible = false
                // push
                toolStack.push(layout)
                parentStack.push(layout.parent)
            }
        }
        function popLayout()
        {
            if (toolStack.length > 0) {
                dockLayout.visible = false
                dockLayout.parent = parentStack.pop()
                //remove layout from stack
                toolStack.pop()
                // return the one we have on top
                return toolStack[toolStack.length - 1]
            }
            return null
        }
    }

    objectName: "Dock"
    styleName: "ThemeToolbarFrame" //"dockFrame"
    width: parent ? parent.width : 0
    height: THEME.sizes.dockHeight
    // mice eater
    MouseArea {anchors.fill: parent}

    // by default doks are shown in the UI; alter this to hide docks
    property bool hidden: false
    states: [
        State {
            name: 'hidden'
            when: dock.hidden
            PropertyChanges {
                target: dock
                opacity: 0.0
            }
            AnchorChanges {
                target: dock
                anchors.top: parent.bottom
                anchors.bottom: undefined
            }
        }

    ]

    // make sure the dock captures group switching
    focusType: StyledItem.FocusGroup
/*
    actions: [
        Action {
            key: Qt.Key_Tab
            onTriggered: focusOnNextGroup()
        },
        Action {
            key: Qt.Key_Backtab
            keyModifier: Qt.ShiftModifier
            onTriggered: focusOnPreviousGroup()
        }

    ]
*/
}
