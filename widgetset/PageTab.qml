// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    id: pageTab

    default property alias content: stack.children
    property variant tabAlign: Qt.AlignTop

    property int current: -1

    onCurrentChanged: privates.setCurrentPage()
    Component.onCompleted: privates.setOpacities()

    QtObject {
        id: privates
        property Item prevPage
        function setOpacities() {
            for (var i = 0; i < stack.children.length; ++i) {
                if (stack.children[i].hasOwnProperty("status"))
                    stack.children[i].status = WidgetSet.PageInactive
                stack.children[i].visible = false
            }
            setCurrentPage()
        }
        function setCurrentPage()
        {
            if ((current < 0) || (current > stack.children.length))
                return
            console.debug("PageTab.setCurrentPage - " + current + ", len - " + stack.children.length)
            if (prevPage) {
                if (prevPage.hasOwnProperty("status")) {
                    prevPage.status = WidgetSet.PageDeactivating
                    prevPage.status = WidgetSet.PageInactive
                }
                prevPage.visible = false
            }
            prevPage = stack.children[current]
            if (prevPage.hasOwnProperty("status")) {
                prevPage.status = WidgetSet.PageActivating
                prevPage.status = WidgetSet.PageActive
            }
            prevPage.visible = true
        }
    }

    Dock {
        id: control
        styleName: "ToolBar"
        width: parent.width
        anchors.top: (tabAlign == Qt.AlignTop) ? parent.top : undefined
        anchors.bottom: (tabAlign == Qt.AlignBottom) ? parent.bottom : undefined
        contentFit: CheckGroup.Custom
        onActiveToolIdChanged: pageTab.current = activeToolId

        Repeater {
            model: stack.children.length
            delegate: ToolButton {
                radio: true
                checkable: true
                buttonId: index
                width: pageTab.width / stack.children.length;
                text: (stack.children[index].hasOwnProperty("title")) ? stack.children[index].title : ""
                checked: pageTab.current == index
            }
        }
    }

    Item {
        id: stack
        width: pageTab.width
        anchors.top: (tabAlign == Qt.AlignTop) ? control.bottom : pageTab.top
        anchors.bottom: (tabAlign == Qt.AlignTop) ? pageTab.bottom : control.top
    }
}