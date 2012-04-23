// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0
import "Utility.js" as Utils

Rectangle {

    /**
      API
      */
    property int fadeInDuration: 175
    property int fadeOutDuration: 175
    property int fadeOutDelay: 100
    property int inputHeight: 180
    property bool usePlatformInput: false
    function openInput(control, inputMethod)
    {
        privates.focusControl = control;
        if (usePlatformInput) {
            privates.focusControl.openSoftwareInputPanel()
            return
        }

        var controlHolder = Utils.namedParent(control, "TextEditor")
        var flickable = Utils.namedParent(control, "Scrollable")
        if (flickable) {
            //flickable.contentY = inputHeight
            console.debug("holder.y= " + controlHolder.y +
                          ", panel.y= " + inputPanel.y +
                          ", flickable(y= " + flickable.y +
                          ", h= " + flickable.height +
                          ", contentY= " + flickable.contentY +
                          ", contentHeight= " + flickable.contentHeight +
                          ")")
            console.debug("panel (" +
                          "y= " + inputPanel.y +
                          ")")
        }
        state = 'visible'
    }
    function closeInput()
    {
        privates.focusControl.focus = false
        state = ''
        privates.focusControl.closeSoftwareInputPanel()
    }

    //privates
    id: inputPanel
    width: parent.width
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: undefined
    anchors.top: parent.bottom
    height: inputHeight
    color: "transparent"
    border.color: "silver"
    border.width: 1
    QtObject {
        id: privates
        property Item focusControl
        property bool isOpen: false
    }

    Image {
        anchors.fill: parent
        anchors.margins: 1
        source: "qrc:/default/keypad"
    }

    states: [
        State {
            name: 'visible'
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
            from: ""; to: "visible"
            AnchorAnimation {alwaysRunToEnd: true; duration: fadeInDuration; easing.type: Easing.OutCubic}
        },
        Transition {
            from: "visible"; to: ""
            SequentialAnimation {
                PauseAnimation { duration: fadeOutDelay }
                AnchorAnimation {alwaysRunToEnd: true; duration: fadeOutDuration; easing.type: Easing.OutCubic}
            }
        }
    ]
}
