import QtQuick 1.0
import com.ui.components 1.0

Page {
    objectName: "Page4"
    onOpened: {hideTools = true; util.busy = true}
    onClosed: {hideTools = false; util.busy = false}
    Column {
        width: parent.width
        Text {
            text: "Page 4"
            color: "white"
            font.bold: true
            font.pixelSize: 20
        }

        PushButton {
            caption: "Get back one view"
            fillColor: "red"
            onClicked: pageStack.prev()
        }
    }
}
