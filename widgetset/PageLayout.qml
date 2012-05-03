/**
  PageLayout - Defines the layout for application and page. Header animation is
  controlled through theme styles. By defaul tit is styled to "ThemePageLayout"
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    id: layout
    // header item
    property alias header: headerItem
    // header layout
    property Item headerLayout
    // for property binding
    property alias headerHidden: headerItem.hidden
    // body content
    default property alias content: bodyItem.data

    // internals
    width: 400
    height: 800
    styleName: "ThemePageLayout"

    onHeaderLayoutChanged: {
        if (headerLayout) {
            headerLayout.parent = headerItem
        }
    }

    Binding {
        target: headerLayout
        property: "styleName"
        value: layout.style.headerStyle
        when: headerLayout != undefined
    }

    Item {
        id: headerItem
        objectName: "HeaderLayout"
        height: layout.style.headerHeight
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        property bool hidden: false

        states: [
            State {
                name: "hidden"
                when: headerItem.hidden
                PropertyChanges {
                    target: headerItem
                    opacity: 0.0
                }
                AnchorChanges {
                    target: headerItem
                    anchors.top: undefined
                    anchors.bottom: parent.top
                }
            }
        ]
        transitions: [
            Transition {
                from: ""
                to: "hidden"
                reversible: true
                ParallelAnimation {
                    AnchorAnimation { easing.type: layout.style.transitionEasing; duration: layout.style.transitionDuration }
                    PropertyAnimation { properties: "opacity"; duration: layout.style.transitionDuration}
                }
            }
        ]
    }

    Item {
        id: bodyItem
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: headerItem.bottom
        anchors.topMargin: 0
    }

}
