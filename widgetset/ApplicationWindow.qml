// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    // default property holding aplication content
    default property alias content: layout.data//content
    // status bas item
    property alias statusBar: statusBarItem
    // page header layout
    property alias header: layout.header
    // current orientation
    property alias orientation: app.orientation
    // string representation of orientation
    property alias orientationString: app.orientationString
    // locked orientation
    property alias lockOnOrientation: app.lockOnOrientation

    /* load the default theme, just to make sure we have a valid
       theme  */
    DefaultTheme {}

    width: 400
    height: 400

    ApplicationItem {
        id: app
        styleName: "ApplicationWindow"
        // properties to store item instances, transfered to native code
        property alias statusBar: statusBarItem
        property alias headerPanel: layout.header
        property alias body: layout.body
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
            styleName: "AppRoundCorners"
            anchors.top: statusBarItem.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            z: Number.MAX_VALUE
        }

        PageLayout {
            id: layout
            anchors.top: statusBarItem.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            styleName: app.style.pageLayoutStyle

            Background {
                parent: header
                styleName: app.style.headerStyle
                anchors.fill: parent
                z: -1
            }
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
            parent: layout.body
        }
    }
}
