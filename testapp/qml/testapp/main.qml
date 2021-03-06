import QtQuick 1.1
import com.zesoft.widgetset 1.0

ApplicationWindow {
    id: app
    width: 800
    height: 600
    //header.visible: false
    title: "Widgetset test app (press for menu)"

    Component.onCompleted: widgetSet.statusBar.hidden = false

    // application menu
    mainMenu: MenuPanel {
        id: menuPanel
        fadeStyle: WidgetSet.FadeTop
        parent: app

        Page {
            parent: menuPanel.panelItem
            anchors.fill: parent
            backgroundStyle: "ThemeMenuBackground"
            headerLayout: PageHeader {
                styleName: "ThemeMenuHeader"
                Button {
                    width: 80
                    text: "Quit"
                    onClicked: Qt.quit()
                }
            }
        }

        bottomMargin: 120
        rightMargin: 40
        leftMargin: 40
    }
    toolBar: toolBarItem
    Dock {
        id: toolBarItem
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
/*
    Page {
        id: page
        title: "single page"
        //headerLayout: PageHeader { styleName: "ThemePageHeader"}
        Button {
            anchors.top: page.contentItem.top
            height: 120
            width: 200
            text: "Top-anchored test"
            onParentChanged: console.debug("Buton parent:"+parent)
        }
        Button {
            anchors.bottom: page.contentItem.bottom
            height: 120
            width: 100
            text: "Test"
            onParentChanged: console.debug("Buton parent:"+parent)
        }
        tools: ToolBarLayout {
            resizeToolsToFit: false
            centerLayout: true
            Button {
                text: "alpha"
            }
            Button {
                text: "beta"
            }
        }
        Component.onCompleted: toolBarItem.setLayout(tools)
    }

*/
    PageTab {
        //currentIndex: 2
        Component.onCompleted: currentPageIndex = 2
        View2 { image:"qrc:/default/quit"; imageActive: "qrc:/default/check-mark"}

        View1 { image:"qrc:/default/quit"; imageActive: "qrc:/default/check-mark"}

        Page {
            image:"qrc:/default/quit"; imageActive: "qrc:/default/check-mark"
            id: embeddedPage
            title: "Page2"
            anchors.fill: parent
            //anchors.topMargin: toolbar.height
            headerLayout: PageHeader {
                styleName: "ThemePageHeader"
                Button {
                    text: "Quit"
                    onClicked: Qt.quit()
                    anchors.left: parent.left
                    anchors.margins: 10
                }
            }

            Column {
                anchors.fill: parent
                spacing: 30
                Label {
                    text: "Page 2"
                    font: THEME.largeFont
                }
                Button {
                    text: "show/hide header"
                    width:200
                    onClicked: embeddedPage.header.hidden = !embeddedPage.header.hidden
                }
                Button {
                    text: "show/hide statusBar"
                    width:200
                    onClicked: widgetSet.statusBar.hidden = !widgetSet.statusBar.hidden
                }
                LineEdit {
                    hint: "whatever"
                }
            }
            Label {
                text: "Bottom anchored label"
                anchors.bottom: parent.bottom
                color: "white"
            }
        }
    }

}

