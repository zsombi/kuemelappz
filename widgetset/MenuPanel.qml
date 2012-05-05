/**
  MenuPanel - panel component to host application menu layout. Follows PageLayout.

  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

FaderItem {
    property int leftMargin: 0
    property int topMargin: 0
    property int rightMargin: 0
    property int bottomMargin: 100

    //property alias headerHidden: menuLayout.headerHidden
    default property alias content: panel.data

    property alias panelItem: panel

    backgroundSource: "qrc:/default/toolbutton-normal"
    id: menuPanel

    Item {
        id: panel
        visible: parent.fadedIn
        anchors.fill: parent
        anchors.leftMargin: leftMargin
        anchors.topMargin: topMargin
        anchors.rightMargin: rightMargin
        anchors.bottomMargin: bottomMargin
        // capture mouse gestures from the panel area
        MouseArea {
            anchors.fill: parent
        }
    }

    /*
    Background {
        styleName: "ThemeMenuBackground"
        MouseArea {
            anchors.fill: parent
        }

        PageLayout {
            id: menuLayout
            visible: menuPanel.fadedIn
            styleName: "ThemeMenuLayout"
            anchors.fill: parent

            headerLayout: PageHeader {
            }
        }
    }
    */

    /*
    Rectangle {
        visible: parent.fadedIn
        anchors.fill: parent
        anchors.margins: 2
        anchors.bottomMargin: marginFromBottom
        color: "blue"
        radius: 5

        MouseArea {
            anchors.fill: parent
        }
    }
    */
}
