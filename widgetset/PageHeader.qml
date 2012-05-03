/**
  PageHeader - component defining page header layout. When added to a page, the
  component is reparented automatically to the headerItem of the page.

  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Background {
    id: headerLayout

    // default property defining the content
    default property alias content: body.data

    anchors.fill: parent
    objectName: "PageHeader"

    Item {
        id: body
        anchors.fill: parent
        anchors.margins: headerLayout.style.headerMargins
    }
}
