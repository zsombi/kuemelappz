// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/**
  * Defines the layout for application and page. Header animation is controlled through theme styles
  */
StyledItem {
    id: layout
    // header item
    property alias header: headerItem
    // for property binding
    property alias headerHidden: headerItem.hidden
    // body item
    property alias body: bodyItem
    // body content
    default property alias content: bodyItem.children

    // internals
    width: 400
    height: 800

    localStyles: [
        Style {
            name: "Layout"
            property int transitionEasing: Easing.InOutExpo
            property int transitionDuration: 800
        }
    ]
    styleName: "Layout"

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
