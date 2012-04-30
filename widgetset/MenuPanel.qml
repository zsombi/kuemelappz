// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

// some day... input is higher in priority
FaderItem {
    backgroundSource: "qrc:/default/toolbutton-normal"
    Rectangle {
        visible: parent.fadedIn
        anchors.fill: parent
        anchors.margins: 10
        anchors.bottomMargin: 100
        color: "blue"
        radius: 5

        MouseArea {
            anchors.fill: parent
        }
    }
}
