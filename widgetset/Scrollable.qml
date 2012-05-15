/**
  Page content scroller.
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    // control flicking even if the content is not reaching out of boundaries
    property bool alwaysScroll: true
    default property alias content: pageContent.data
    clip: true

    //anchors.fill: parent
    Flickable {
        objectName: "Scrollable"
        id: scrollable
        anchors.fill: parent
        anchors.margins: THEME.sizes.spacingMedium

        flickableDirection: Flickable.VerticalFlick
        interactive: alwaysScroll || (pageContent.height > height)
        contentWidth: width
        contentHeight: pageContent.childrenRect.height

        Item {
            id: pageContent
            width: parent.width
            //height: childrenRect.height
        }
    }
    ScrollBarDecorator {
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

