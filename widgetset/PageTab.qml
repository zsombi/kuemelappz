/**
  PageTab - tabbed page stack.

  All the pages handled by the tab are created upon application initialization.
  Use Loader to delay page initialization if needed.

  PageTab does not provide any transition effect when navigating between pages.

  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Item {
    id: pageTab

    //page content
    default property alias content: stack.data
    // tab button style
    property string tabButtonStyle: "PageTabButton"
    // tab height
    property int tabHeight: THEME.sizes.pageTabHeight
    // current page Index
    property int currentIndex: -1
    // control page title visibility in tab control
    property bool hidePageTitles: false

    onCurrentIndexChanged: privates.setCurrentPage()
    Component.onCompleted: privates.initialize()

    anchors.fill: parent
    anchors.bottomMargin: (parent.parent.toolBar !== undefined && parent.parent.toolBar) ? parent.parent.toolBar.height : 0

    QtObject {
        id: privates
        property Item prevPage
        function initialize() {
            for (var i = 0; i < stack.children.length; ++i) {
                if (stack.children[i].hasOwnProperty("status"))
                    stack.children[i].status = WidgetSet.PageInactive
                stack.children[i].opacity = 0.0
                stack.children[i].visible = false
            }
            setCurrentPage()
            widgetSet.toolBar.setLayout(tabBar)
        }
        function setCurrentPage()
        {
            if ((currentIndex < 0) || (currentIndex > stack.children.length))
                return
            //console.debug("PageTab.setCurrentPage - " + currentIndex + ", len - " + stack.children.length)
            if (prevPage) {
                if (prevPage.hasOwnProperty("status")) {
                    prevPage.status = WidgetSet.PageDeactivating
                    prevPage.status = WidgetSet.PageInactive
                }
                prevPage.opacity = 0.0
                prevPage.visible = false
            }
            prevPage = stack.children[currentIndex]
            if (prevPage.hasOwnProperty("status")) {
                prevPage.status = WidgetSet.PageActivating
                prevPage.status = WidgetSet.PageActive
            }
            prevPage.opacity = 1.0
            prevPage.visible = true
        }
    }

    CheckGroup {
        onActiveIdChanged: pageTab.currentIndex = activeId
        ToolBarLayout {
            id: tabBar
            resizeToolsToFit: true
            //centerLayout: true
            Repeater {
                objectName: "NonVisualElement"
                anchors.fill: parent
                model: stack.children.length
                delegate: ToolButton {
                    styleName: tabButtonStyle
                    radio: true
                    checkable: true
                    buttonId: index
                    //width: pageTab.width / stack.children.length;
                    image: (stack.children[index].hasOwnProperty("image")) ? stack.children[index].image : ""
                    imageActive: (stack.children[index].hasOwnProperty("imageActive")) ? stack.children[index].imageActive : ""
                    text: (stack.children[index].hasOwnProperty("title") && !pageTab.hidePageTitles) ? stack.children[index].title : ""
                    checked: pageTab.currentIndex == index
                }
            }
        }
    }
    Item {
        id: stack
        width: pageTab.width
        anchors.fill: parent
    }
}
