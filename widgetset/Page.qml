/**
  Page - page element.
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    id: page
    // page title string - used by the PageTab to display and identify page
    property string title
    // page image - displayed by the PageTab is inactive
    property url image: ""
    // page image - displayed by the PageTab when page is active
    property url imageActive: ""
    // page status
    property int status: WidgetSet.PageInactive
    // style of the background
    property string backgroundStyle
    // PageStack component set if the page is included in a page stack
    property PageStack pageStack
    // page data holder
    default property alias content: layout.content
    property alias header: layout.header

    // signal emitted when page got opened
    signal opened()
    // signal emitted when page gets closed
    signal closed()

    anchors.fill: parent

    //controlListItem: body
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

    PageLayout {
        id: layout
        anchors.fill: parent
        styleName: "PageLayout"
        headerHidden: widgetSet.headerPanel.hidden
    }
}
