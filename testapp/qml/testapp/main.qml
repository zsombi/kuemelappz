import QtQuick 1.1
import com.zesoft.widgetset 1.0

ApplicationWindow {
    id: app
    width: 640
    height: 480
    //header.visible: false
    objectName: "AppWin"

    PageTab {
        anchors.fill: parent
        tabAlign: Qt.AlignBottom
        current: 2
        View2 {
        }

        View1 {

        }

        Page {
            title: "Page2"
            anchors.fill: parent
            //anchors.topMargin: toolbar.height
            header.children: [
                Button {
                    text: "Quit"
                    onClicked: Qt.quit()
                    anchors.left: parent.left
                    anchors.margins: 10
                }
            ]

            Column {
                spacing: 10
                Label {
                    text: "Page 2"
                    font.pixelSize: THEME.textSizeLarge
                }
                Button {
                    text: "show/hide header"
                    width:200
                    onClicked: widgetSet.headerPanel.hidden = !widgetSet.headerPanel.hidden
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

