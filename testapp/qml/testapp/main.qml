import QtQuick 1.1
import com.zesoft.widgetset 1.0

ApplicationWindow {
    id: app
    width: 640
    height: 480
    //header.visible: false
    objectName: "AppWin"

    // application menu
    mainMenu: MenuPanel {
        id: menuPanel
        fadeStyle: WidgetSet.FadeTop
        parent: app
        headerHidden: true
        //parent: widgetSet.applicationBody
        bottomMargin: 120
        rightMargin: 40
    }

    PageTab {
        anchors.fill: parent
        tabAlign: Qt.AlignBottom
        //currentIndex: 2
        Component.onCompleted: currentIndex = 1
        View2 { image:"qrc:/default/quit"; imageActive: "qrc:/default/check-mark"}

        View1 { image:"qrc:/default/quit"; imageActive: "qrc:/default/check-mark"}

        Page {
            image:"qrc:/default/quit"; imageActive: "qrc:/default/check-mark"
            id: embeddedPage
            title: "Page2"
            anchors.fill: parent
            //anchors.topMargin: toolbar.height
            headerLayout: PageHeader {
                anchors.fill: parent
                Button {
                    text: "Quit"
                    onClicked: Qt.quit()
                    anchors.left: parent.left
                    anchors.margins: 10
                }
            }

            Column {
                spacing: 10
                Label {
                    text: "Page 2"
                    font.pixelSize: THEME.textSizeLarge
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
            }
        }

    }
}

