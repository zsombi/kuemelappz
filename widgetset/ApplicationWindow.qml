// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    // default property holding aplication content
    default property alias content: layout.data
    // status bas item
    property alias statusBar: statusBarItem
    // current orientation
    property alias orientation: app.orientation
    // string representation of orientation
    property alias orientationString: app.orientationString
    // locked orientation
    property alias lockOnOrientation: app.lockOnOrientation
    // alter this property if system's input panel is required
    property bool usePlatformInput: false
    // menu panel
    property alias mainMenu: statusBarItem.menu

    /* load the default theme, just to make sure we have a valid
       theme  */
    DefaultTheme {}

    width: 400
    height: 400

    ApplicationItem {
        id: app
        styleName: "ThemeApplicationWindow"
        // properties to store item instances, transfered to native code
        property alias statusBar: statusBarItem
        property alias body: layout
        property alias inputPanel: inputPanelItem

        anchors.centerIn: parent
        StatusBar {
            id: statusBarItem
            width: parent.width
            anchors.top: parent.top
            visible: widgetSet.mobilePlatform
            // be the topmost component
            z: Number.MAX_VALUE
        }

        Background {
            id: background
            anchors.fill: parent
            styleName: app.style.backgroundStyle
        }

        CornerFramer {
            styleName: "ThemeAppRoundCorners"
            anchors.top: statusBarItem.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            z: Number.MAX_VALUE
        }

        // content item
        Item {
            id: layout
            anchors.top: statusBarItem.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }

        // animate rotation
        Behavior on rotation {
            RotationAnimation {
                direction: RotationAnimation.Shortest
                duration: app.style.rotationDuration
                easing.type: app.style.rotationEasing
            }
        }
        // input panel
        InputPanel {
            id: inputPanelItem
            styleName: app.style.inputPanelStyle
            parent: layout
        }
    }
}
