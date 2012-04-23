// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Rectangle {
    id: statusBar
    width: 200
    height: 12
    color: theme.style.fillColor
    //todo: add sensing to open menu panel when pressed or swiped

    MouseArea {
        id: tap
        anchors.fill: parent
        onClicked: {menuPanel.open(WidgetSet.FadeFromTop)}
    }

    MenuPanel {
        id: menuPanel
        //fadeStyle: WidgetSet.FadeFromTop
        parent: widgetSet.rootBodyItem()
    }

    StyledItem {
        id: theme
        styleName: "StatusBar"
        anchors.fill: parent
        Label {
            id: dateTimeText
            width: 52
            height: 26
            color: theme.style.fontColor
            font.family: theme.style.fontFamily
            font.weight: theme.style.fontWeight
            font.pixelSize: theme.style.fontPixels
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            text: getDateTime()

            function getDateTime() {
                var dateTime = new Date();
                return Qt.formatDateTime(dateTime, "hh:mm");
            }
            Timer {
                id: dateTimer
                interval: 10000
                running: true
                repeat: true
                onTriggered: {
                    dateTimeText.text = dateTimeText.getDateTime();
                }
            }
        }
        BusyIndicator {
            height: statusBar.height
            width: statusBar.height
            anchors.left: dateTimeText.right
            on: true
        }
    }
}
