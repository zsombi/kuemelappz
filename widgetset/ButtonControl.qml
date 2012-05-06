import QtQuick 1.1
import "." 1.0

StyledItem {
    //public API
    // set to true if radio button
    property bool radio: false
    // check state
    property bool checked: false
    // is checkable
    property bool checkable: false
    // pressed state
    property bool pressed: false
    property bool highlighted: pressed
    // auto-repeat
    property bool autoRepeat: false
    // emits long-presses?
    property bool longPress: false
    // button identifier
    property int buttonId: -1
    // do auto-focus on pressed or not
    property bool autoFocusOnPress: true
    // focused property, true when it is the active button
    property bool focused: false
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
            if (autoFocusOnPress)
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
                if (autoFocusOnPress)
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
