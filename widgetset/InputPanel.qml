// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0
import "Utility.js" as Utils

// InputPanel - panel for key inputs  for mobile platforms.
// UNDER CONSTRUCTION

StyledItem {

    /**
      API
      */
    // app screen proportion to be used when faded in
    //property int inputHeight: 20

    function openInput(control, flags, inputMethod)
    {
        // return if we are not in mobile platform
        if (!widgetSet.mobilePlatform)
            return
        // the control is the TextInput editor control
        privates.flags = flags
        privates.focusControl = control;
        privates.rollUp()
    }
    function closeInput()
    {
        // loose focus
        privates.focusControl.focus = false
        privates.isOpen = false
    }

    // handle control scrollup
    onYChanged: privates.adjustFocusControlPosition()

    //privates
    id: inputPanel
    width: parent.width
    height: THEME.sizes.inputPanelHeight
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: undefined
    anchors.top: parent.bottom
    onHeightChanged: console.debug("panelHeight="+height)

    QtObject {
        id: privates
        // panel content flags
        property int flags
        // this is the editor that opened the input panel
        property Item focusControl
        // this is a StyledItem containing the control
        property Item holder
        // the layout containing the holder
        property Item holderLayout
        // set to true if the holder layout is Scrolable
        property bool scrollableHolderLayout: false
        // bottom Y screen point of the holder, needed to keep control in visible area
        // when the panel is rolled up
        property int holderBottom
        // drives the roll-up
        property bool isOpen: false
        // to know when are we still in animation phase
        property bool animating: false
        // margin to be applied when focus control is

        function rollUp()
        {
            // the holder is named as TextEditor
            holder = Utils.namedParent(focusControl, "TextEditor")
            holderBottom = Utils.screenY(holder) + holder.height
            // check if the holder layout is a Scrollable
            holderLayout = Utils.namedParent(holder, "Scrollable")
            //console.debug("rollUp:: focus="+focusControl+", holder="+holder+", layout="+holderLayout)
            scrollableHolderLayout = (holderLayout != null)
            if (!holderLayout) {
                // it is not, so it might be a positioner (TODO: what if not?)
                holderLayout = holder.parent
            }
            isOpen = true
        }
        // adjusts focus control position on the screen, so it is visible even when the
        // panel is shown
        function adjustFocusControlPosition()
        {
            if (!focusControl || !animating)
                return

            var inputSy = Utils.screenY(inputPanel)
            var currBottom = Utils.screenY(holder) + holder.height
            var diff = inputSy - currBottom

            if (isOpen) {
                // roll up
                adjustHolder(diff)
            } else {
                // roll down
                if (diff > (holderBottom - currBottom))
                    diff = (holderBottom - currBottom)
                adjustHolder(diff)
            }
        }
        // move holder layout
        function adjustHolder(points)
        {
            if (isOpen) {
                if (points < 0) {
                    if (scrollableHolderLayout)
                        holderLayout.contentY -= points
                    else {
                        holderLayout.anchors.topMargin += parseInt(points)
                        console.debug("adjustHolder topMargin="+holderLayout.anchors.topMargin)
                    }
                }
            } else {
                if (points > 0) {
                    if (scrollableHolderLayout)
                        holderLayout.contentY -= points
                    else
                        holderLayout.anchors.topMargin += parseInt(points)
                } else {
                    holderLayout.anchors.topMargin = 0
                }
            }
        }

        function clearPanel()
        {
            animating = false
            focusControl = null
            scrollableHolderLayout = false
            flags = 0
        }
    }

    // hide it if not open; without this the panel is visible upon rotation7
    visible: privates.isOpen
    // eat presses!
    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        id: stripe
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.right: parent.right
        height: 1
        border.color: inputPanel.style.borderColor
        border.width: 1
        color: inputPanel.style.borderColor
    }

    Image {
        anchors.left: parent.left
        anchors.top: stripe.bottom
        anchors.topMargin: 1
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        source: inputPanel.style.backgroundImageUrl
        // todo: put this out in layout definition document
        Item {
            id: layout
            anchors.fill: parent
            anchors.margins: 3

            property int rows: 4
            property real keyHeight: height/rows - THEME.sizes.spacingNormal//4 rows
            Button {
                styleName: inputPanel.style.controlButtonStyle
                id: btnClose
                width: 100
                height: layout.keyHeight
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                autoFocusOnPress: false
                text: qsTr("Done")
                onClicked: closeInput()
            }
        }
    }


    states: [
        State {
            name: 'rollUp'
            when: privates.isOpen
            AnchorChanges {
                target: inputPanel
                anchors.bottom: parent.bottom
                anchors.top: undefined
            }
        }
    ]
    transitions: [
        Transition {
            from: ""; to: "rollUp"
            SequentialAnimation {
                ScriptAction {script: privates.animating = true;}
                AnchorAnimation {alwaysRunToEnd: true; duration: inputPanel.style.fadeInDuration; easing.type: inputPanel.style.fadeEasing}
                ScriptAction {script: privates.animating = false;}
            }
        },
        Transition {
            from: "rollUp"; to: ""
            SequentialAnimation {
                ScriptAction {script: privates.animating = true;}
                PauseAnimation { duration: inputPanel.style.fadeOutDelay }
                AnchorAnimation {alwaysRunToEnd: true; duration: inputPanel.style.fadeOutDuration; easing.type: inputPanel.style.fadeEasing}
                ScriptAction {script: privates.clearPanel()}
            }
        }
    ]
}
