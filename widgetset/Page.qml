// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    id: page
    property string title
    property bool active: false
    property int status: WidgetSet.PageInactive
    property url backgroundImage: UI.pageBackgroundImage
    property color backgroundColor
    //property PageStack pageStack
    default property alias content: body.data
    signal opened()
    signal closed()

    anchors.fill: parent

    controlListItem: body
    onStatusChanged: {
        if (status == WidgetSet.PageActive)
            opened()
        else if (status == WidgetSet.PageInactive)
            closed()
    }

    Rectangle {
        z: -1
        anchors.fill: parent
        clip: true
        color: (!page.backgroundImage && (page.backgroundImage == "")) ? page.backgroundColor : "transparent"
        Image {
            anchors.fill: parent
            source: page.backgroundImage
        }
    }
    Item {
        id: body
        focus: true
        anchors.fill: parent
        anchors.margins: 2
    }
}
