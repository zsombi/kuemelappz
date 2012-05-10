// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    // default property holding aplication content
    default property alias content: layout.data
    // status bas item
    property alias statusBar: statusBarItem
    // current orientation
    property int orientation: screen.orientation
    // string representation of orientation
    property string orientationString: screen.orientationString
    // locked orientation
    property int lockOnOrientation: screen.lockOnOrientation
    // alter this property if system's input panel is required
    property bool usePlatformInput: false
    // menu panel
    property alias mainMenu: statusBarItem.menu

    /* load the default theme, just to make sure we have a valid
       theme  */
    DefaultTheme {}

    Connections {
        target: screen
        onDisplayChanged: console.debug("screenW= "+screen.width+", screenH= "+screen.height)
    }

    ApplicationItem {
        id: app
        styleName: "ThemeApplicationWindow"
        property bool animate: false
        // properties to store item instances, transfered to native code
        property alias statusBar: statusBarItem
        property alias body: layout
        property alias inputPanel: inputPanelItem

        anchors.centerIn: parent
        StatusBar {
            id: statusBarItem
            styleName: app.style.statusBarStyle
            width: parent.width
            anchors.top: parent.top
            hidden: !widgetSet.mobilePlatform
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

        // input panel
        InputPanel {
            id: inputPanelItem
            styleName: app.style.inputPanelStyle
            parent: layout
            //height: app.orientation == ApplicationItem.Portrait ? 440 : 200
        }

        Behavior on width {
            PropertyAnimation { duration: app.style.rotationDuration; easing.type: app.style.rotationEasing }
        }
        Behavior on height {
            PropertyAnimation { duration: app.style.rotationDuration; easing.type: app.style.rotationEasing }
        }

        // rotate and animate
        Behavior on rotation {
            SequentialAnimation {
                //alwaysRunToEnd: true
                PauseAnimation { duration: app.style.rotationStartPause }
                PropertyAnimation { target: app; property: "scale"; from: 1.0; to: app.style.animationScaling; duration: app.style.rotationDuration}
                RotationAnimation {
                    direction: RotationAnimation.Shortest
                    duration: app.style.rotationDuration
                    easing.type: app.style.rotationEasing
                }
                PropertyAnimation { target: app; property: "scale"; from: app.style.animationScaling; to: 1.0; duration: app.style.rotationDuration}
            }
        }
    }
}
