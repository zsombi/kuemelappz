// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    //application title string displayed in the status bar or to its system bar on desktops
    property alias title: app.title
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
    // set whether the app has a toolbar or not; the toolbar is always in the bottom of the screen
    property Item toolBar: null

    /* load the default theme, just to make sure we have a valid
       theme  */
    DefaultTheme {objectName:"DefaultTheme"}

    id: appHolder
    objectName: "ApplicationWindowItem"
    // ApplicationItem handles rotation of the layout upon orientation changes. Here
    // we set the layout elements needed by the entire system.
    ApplicationItem {
        id: app
        //focus: true
        objectName: "FC_ApplicationItem"
        styleName: "ThemeApplicationWindow"
        property bool animate: false
        // properties to store item instances, transfered to native code
        property alias statusBar: statusBarItem
        property alias body: layout
        //property alias inputPanel: inputPanelItem
        property alias toolBar: appHolder.toolBar

        anchors.centerIn: parent
        StatusBar {
            id: statusBarItem
            application: app
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

        // content item
        Item {
            id: layout
            objectName: "AppWindowBody"
            anchors.top: statusBarItem.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }

        CornerFramer {
            styleName: "ThemeAppRoundCorners"
            anchors.top: statusBarItem.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            z: Number.MAX_VALUE
        }

        // input panel
        /*
        InputPanel {
            id: inputPanelItem
            styleName: app.style.inputPanelStyle
            parent: layout
            // just below the corners
            z: Number.MAX_VALUE - 1
        }
        */

        Behavior on width {
            PropertyAnimation { duration: app.style.scalingDuration; easing.type: app.style.rotationEasing }
        }
        Behavior on height {
            SequentialAnimation {
                PropertyAnimation { duration: app.style.scalingDuration; easing.type: app.style.rotationEasing }
                // adjust opened input panel so that
                //ScriptAction {script: inputPanelItem.update()}
            }
        }

        // rotate and animate
        Behavior on rotation {
            SequentialAnimation {
                //alwaysRunToEnd: true
                PauseAnimation { duration: app.style.rotationStartPause }
                PropertyAnimation { target: app; property: "scale"; from: 1.0; to: app.style.animationScaling; duration: app.style.scalingDuration}
                RotationAnimation {
                    direction: RotationAnimation.Shortest
                    duration: app.style.rotationDuration
                    easing.type: app.style.rotationEasing
                }
                PropertyAnimation { target: app; property: "scale"; from: app.style.animationScaling; to: 1.0; duration: app.style.scalingDuration}
            }
        }
    }
}
