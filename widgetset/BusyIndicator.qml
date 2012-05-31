/**
  BusyIndicator - component displaying busy indication. The indicator image is
  specified in "Indicators" style
*/
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    // drive busy indication
    property bool on: false
    styleName: "Indicators"
    objectName: "SI_BusyIndicator"

    id: busyIndicator
    Image {
        id: container
        anchors.fill: parent
        source: busyIndicator.style.busyIndicatorUrl
        visible: busyIndicator.on
        fillMode: Image.Stretch
        NumberAnimation on rotation { running: busyIndicator.on; from: 0; to: 360; loops: Animation.Infinite; duration: 1200 }
    }
}

