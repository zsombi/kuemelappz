import QtQuick 1.1
import com.zesoft.widgetset 1.0

ApplicationWindow {
    id: app
    width: 640
    height: 480
    header.visible: false
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

            Column {
                spacing: 10
                Label {
                    text: "Page 2"
                    font.pixelSize: UI.textSizeLarge
                }
                Button {
                    text: "Quit"
                    onClicked: {console.debug("QUIT, root = " + widgetSet.rootBodyItem()); Qt.quit()}
                }
            }
        }

    }
}

