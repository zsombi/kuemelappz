// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    id: page
    property string title
    property int status: WidgetSet.PageInactive
    property string backgroundStyle
    property PageStack pageStack
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

    Background {
        styleName: backgroundStyle
        anchors.fill: parent
    }

    Item {
        id: body
        focus: true
        anchors.fill: parent
        anchors.margins: 2
    }
}
