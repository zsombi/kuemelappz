// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    property bool on: false
    styleName: "Indicators"

    id: busyIndicator
    //width: container.sourceSize.width
    //height: container.sourceSize.height
    Image {
        id: container
        anchors.fill: parent
        source: busyIndicator.style.busyIndicatorUrl
        visible: busyIndicator.on
        fillMode: Image.Stretch
        NumberAnimation on rotation { running: busyIndicator.on; from: 0; to: 360; loops: Animation.Infinite; duration: 1200 }
    }
}

