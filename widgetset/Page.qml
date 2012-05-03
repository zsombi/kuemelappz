/**
  Page - page element. Encapsulates the page layout.
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
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
    property alias backgroundStyle: background.styleName
    // page layout style
    property alias layoutStyle: layout.styleName
    // page data holder
    default property alias content: layout.content
    // page header component
    property alias header: layout.header
    // header layout; if not set, header is hidden automatically
    property alias headerLayout: layout.headerLayout
    // PageStack component set if the page is included in a page stack
    property PageStack pageStack

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
        id: background
        anchors.fill: parent
    }

    PageLayout {
        id: layout
        anchors.fill: parent
        // header is invisible if no layout got connected to it
        headerHidden: (layout.headerLayout == null)
    }
}
