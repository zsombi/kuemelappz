// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    property bool autohide: false
    property variant flickableItem
    property variant orientation: Qt.Vertical
    // style properties
    styleName: "ScrollBar"

    id: scrollbar
    anchors.margins: 1
    opacity: internal.moving * 1.0

    QtObject {
        id: internal
        property bool moving: (!autohide) ? true : flickableItem.moving
        function position() {
            var ny = 0;
            if (scrollbar.orientation == Qt.Vertical) {
                ny = flickableItem.visibleArea.yPosition * scrollbar.height;
            } else {
                ny = flickableItem.visibleArea.xPosition * scrollbar.width;
            }
            if (ny > 2) {
                return ny;
            } else {
                return 2;
            }
        }
        function size() {
            var nh, ny;
            if (scrollbar.orientation == Qt.Vertical) {
                nh = flickableItem.visibleArea.heightRatio * scrollbar.height;
            } else {
                nh = flickableItem.visibleArea.widthRatio * scrollbar.width;
            }
            if (scrollbar.orientation == Qt.Vertical) {
                ny = flickableItem.visibleArea.yPosition * scrollbar.height;
            } else {
                ny = flickableItem.visibleArea.xPosition * scrollbar.width;
            }
            if (ny > 3) {
                var t;
                if (scrollbar.orientation == Qt.Vertical) {
                    t = Math.ceil(scrollbar.height - 3 - ny);
                } else {
                    t = Math.ceil(scrollbar.width - 3 - ny);
                }
                if (nh > t) {
                    return t;
                } else {
                    return nh;
                }
            } else {
                return nh + ny;
            }
        }
    }

    states:[
        State {
            name: "moving"
            when: internal.moving
            PropertyChanges {
                target: scrollbar
                opacity: 1.0
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "moving"
            NumberAnimation { properties: "opacity"; duration: scrollbar.style.fadeInDuration }
        },
        Transition {
            from: "moving"
            to: ""
            SequentialAnimation {
                PauseAnimation { duration: scrollbar.style.fadeOutDelay }
                NumberAnimation { properties: "opacity"; duration: scrollbar.style.fadeOutDuration }
            }
        }
    ]

    Rectangle {
        id: frame
        color: scrollbar.style.frameColor
        opacity: scrollbar.style.frameOpacity
        anchors.fill: parent
    }
    Rectangle {
        id: bar
        color: scrollbar.style.barColor
        smooth: true
        radius: 3
        width: scrollbar.orientation == Qt.Vertical ? scrollbar.width - 4 : internal.size()
        height: scrollbar.orientation == Qt.Vertical ? internal.size() : scrollbar.height - 4
        x: scrollbar.orientation == Qt.Vertical ? 2 : internal.position()
        y: scrollbar.orientation == Qt.Vertical ? internal.position() : 2
    }
}
