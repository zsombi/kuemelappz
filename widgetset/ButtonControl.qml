import QtQuick 1.1
import "." 1.0

StyledItem {
    //public API
    property bool radio: false      // set to true if radio button
    //property bool enabled: true     //overrides Item's enabled
    property bool checked: false    // check state
    property bool checkable: false  // is checkable
    property bool pressed: false    // pressed state
    property bool highlighted: pressed
    property bool autoRepeat: false // auto-repeat
    property bool longPress: false  // emits long-presses?
    property int buttonId: -1       // button identifier
    property bool focused: false    // focused property, true when it is the active button
    //property bool grouped: (group !== null)

    signal clicked
    signal released
    signal pressAndHold

    id: buttonControl
    width: 100
    height: 62
    // todo: put dimming into the style!!
    opacity: buttonControl.enabled ? 1.0 : 0.5

    Timer {
        id: tapRepeatTimer

        interval: 1000
        running: false
        repeat: true
        onTriggered: buttonControl.clicked()
    }

    MouseArea {
        id: buttonActive
        hoverEnabled: false
        enabled: buttonControl.enabled
        anchors.fill: parent
        anchors.margins: -2
        onPressed: activate(mouse)
        onCanceled: deactivate()
        onExited: deactivate()
        onReleased: release(mouse)
        onPressAndHold: repeat(mouse)

        function activate(mouse)
        {
            buttonControl.pressed = true
            buttonControl.forceActiveFocus()
            if (!buttonControl.checkable && buttonControl.autoRepeat && !buttonControl.longPress)
                tapRepeatTimer.restart()
        }
        function deactivate(mouse)
        {
            if (!buttonControl.pressed && buttonControl.radio)
                return
            buttonControl.pressed = false
            tapRepeatTimer.stop()
        }
        function release(mouse)
        {
            if (buttonControl.pressed) {
                buttonControl.pressed = false
                buttonControl.focused = true
                if (buttonControl.checkable) {
                    if (radio) {
                        if (!buttonControl.checked)
                            buttonControl.checked = true
                    } else
                        buttonControl.checked = !buttonControl.checked
                } else
                    buttonControl.clicked()
            }
        }
        function repeat(mouse)
        {
            if (buttonControl.longPress && buttonControl.pressed && !buttonControl.checkable) {
                buttonControl.pressAndHold()
            }
        }
    }
}
