/**
  StatusBar - component hosting indicators and other system elements. In addition
  can bring up application menu panel.
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    id: statusBar
    // application instance holding the status bar
    property ApplicationItem application: parent
    // scontrols statsu bar visibility
    property bool hidden: false
    // menu panel item to be shown
    property MenuPanel menu

    width: parent.width
    styleName: "ThemeStatusBar"
    focusType: StyledItem.Decorative
    objectName: "SI_StatusBar"

    anchors.left: parent.left
    anchors.top: parent.top
    anchors.right: parent.right
    height: THEME.sizes.statusBarHeight

    // mouse area to capture gesture activating panel menu
    MouseArea {
        id: tapArea
        anchors.fill: parent
        onClicked: {
            if (menu) {
                if (!menu.fadedIn)
                    menu.show(WidgetSet.FadeTop)
                else
                    menu.hide()
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: statusBar.style.fillColor
        Label {
            id: dateTimeText
            width: paintedWidth
            height: parent.height
            color: statusBar.style.fontColor
            font: statusBar.style.font
            verticalAlignment: Text.AlignVCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
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
            /*
            anchors.left: dateTimeText.right
            anchors.leftMargin: THEME.sizes.spacingMedium
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            */
            anchors.centerIn: parent
            on: widgetSet.busy
        }
        Label {
            id: appTitle
            width: paintedWidth
            height: parent.height
            color: statusBar.style.fontColor
            font: statusBar.style.font
            verticalAlignment: Text.AlignVCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            horizontalAlignment: Text.AlignLeft
            text: (application) ? application.title : ""
        }
    }

    states: [
        State {
            name: "hidden"
            when: statusBar.hidden
            PropertyChanges {
                target: statusBar
                opacity: 0.0
            }
            AnchorChanges {
                target: statusBar
                anchors.top: undefined
                anchors.bottom: parent.top
            }
        }
    ]
    transitions: [
        Transition {
            from: ""
            to: "hidden"
            reversible: true
            ParallelAnimation {
                AnchorAnimation { easing.type: statusBar.style.transitionEasing; duration: statusBar.style.transitionDuration }
                PropertyAnimation { properties: "opacity"; duration: statusBar.style.transitionDuration}
            }
        }
    ]
}
