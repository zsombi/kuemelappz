// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    default property alias body: bodyItem.children
    property alias statusBar: statusBarItem
    property alias header: pageHeaderItem
    property alias orientation: app.orientation
    property alias orientationString: app.orientationString
    property alias lockToOrientation: app.lockToOrientation

    /* load the default theme, just to make sure we have a valid
       theme  */
    DefaultTheme {}

    width: 400
    height: 400

    ApplicationItem {
        id: app
        //rotation: 0
        anchors.centerIn: parent
        StatusBar {
            id: statusBarItem
            width: parent.width
            anchors.top: parent.top
            visible: widgetSet.mobilePlatform
            height: visible ? 13 : 0
        }

        Image {
            anchors.fill: parent
            anchors.topMargin: statusBarItem.height
            source: UI.appBackgroundImage
            fillMode: Image.Tile
        }

        CornerFramer {
            styleName: "AppRoundCorners"
            anchors.fill: parent
            anchors.topMargin: statusBarItem.height
            z: Number.MAX_VALUE
        }

        Rectangle {
            id: pageHeaderItem
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: statusBarItem.bottom
            height: visible ? 55 : 0
            color: "#EA650A"
        }

        Item {
            id: bodyItem
            objectName: "AppBodyItem"
            anchors.fill: parent
            anchors.topMargin: statusBarItem.height + pageHeaderItem.height
        }
        // rotation behavios is defined here
        Behavior on rotation {
            RotationAnimation {
                direction: RotationAnimation.Shortest
                duration: UI.appRotationDuration
                easing.type: Easing.InOutSine
            }
        }

        inputPanel: InputPanel {
            parent: bodyItem
        }
    }
}
