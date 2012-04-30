/**
  Page content scroller.
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    // enable or disable content scrolling
    property bool enableScrolling: true
    // enable automatic scroll
    property bool alwaysScroll: false
    default property alias content: pageContent.data
    clip: true

    anchors.fill: parent
    Flickable {
        objectName: "Scrollable"
        id: scrollable
        anchors.fill: parent
        anchors.margins: THEME.paddingMedium

        flickableDirection: Flickable.VerticalFlick
        interactive: alwaysScroll || (enableScrolling && (pageContent.height > height))
        contentWidth: width
        contentHeight: pageContent.height

        Item {
            id: pageContent
            width: parent.width
            height: childrenRect.height
        }
        z: 0
    }
    ScrollBar {
        id: pageScroll
        flickableItem: scrollable
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 8
        orientation: Qt.Vertical
        autohide: true
    }
}

