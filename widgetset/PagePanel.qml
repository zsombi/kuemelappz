/**
  PagePanel - panel component to hold page panel layouts (Toolbar, Header).
  Internal component.
  The panel is anchored to the page boundaries.
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    // API
    // specifies whether the panel is hidden or not
    property bool hidden: false
    // alignment
    property int alignment: Qt.AlignTop
    // transition properties
    property int transitionEasing:0
    property int transitionDuration:0
    // internals
    id: pagePanel
    anchors.top: (alignment == Qt.AlignTop) ? parent.top : undefined
    anchors.bottom: (alignment == Qt.AlignBottom) ? parent.bottom : undefined
    anchors.right: parent.right
    anchors.left: parent.left

    states: [
        State {
            name: "hidden"
            when: pagePanel.hidden
            PropertyChanges {
                target: pagePanel
                opacity: 0.0
            }
            AnchorChanges {
                target: pagePanel
                anchors.top: {
                    if (alignment == Qt.AlignTop)
                        return undefined
                    else if (alignment == Qt.AlignBottom)
                        return parent.bottom
                    return parent.top
                }
                anchors.bottom: {
                    if (alignment == Qt.AlignTop)
                        return parent.top
                    else if (alignment == Qt.AlignBottom)
                        return undefined
                    return parent.bottom
                }
            }
        }
    ]
    transitions: [
        Transition {
            from: ""
            to: "hidden"
            reversible: true
            ParallelAnimation {
                AnchorAnimation { easing.type: pagePanel.transitionEasing; duration: pagePanel.transitionDuration }
                PropertyAnimation { properties: "opacity"; duration: pagePanel.transitionDuration}
            }
        }
    ]
}

