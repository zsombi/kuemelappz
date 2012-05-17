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
    // page header style
    property real headerHeight: THEME.sizes.headerHeight
    // page header component
    property alias header: headerItem
    // header layout; if not set, header is hidden automatically
    property Item headerLayout
    // set header hidden if layout is empty
    property bool hideHeaderWhenEmpty: true

    // PageStack component set if the page is included in a page stack
    property PageStack pageStack
    // tools used in the page
    property Item tools
    // property specifying the content item, needed to anchor content
    property alias contentItem: bodyItem
    // page data holder
    default property alias content: bodyItem.data

    // signal emitted when page got opened
    signal opened()
    // signal emitted when page gets closed
    signal closed()

    // make it as fullpage, but in case of parent is the app window, check the toolbar presence
    anchors.fill: parent
    anchors.bottomMargin: (parent.parent.toolBar !== undefined && parent.parent.toolBar) ? parent.parent.toolBar.height : 0

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

    // page header
    PagePanel {
        id: headerItem
        alignment: Qt.AlignTop
        objectName: "HeaderPanel"
        height: headerHeight
        transitionEasing: THEME.panelFadingEasing
        transitionDuration: THEME.panelFadingDuration
        hidden: (hideHeaderWhenEmpty) ? (headerLayout == undefined) : false
    }
    onHeaderLayoutChanged: {
        if (headerLayout) {
            headerLayout.parent = headerItem
        }
    }

    Item {
        id: bodyItem
        clip: true
        anchors.top: headerItem.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: THEME.sizes.spacingSmall
    }
}
