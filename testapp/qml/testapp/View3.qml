import QtQuick 1.0
import com.zesoft.widgetset 1.0

Page {
    width: pgStack.width
    height: pgStack.width

    Column {
        anchors.fill: parent
        spacing: 10
        Button {
            text: "Back"
            onClicked: pgStack.back()
        }
        Button {
            text: "Next"
            onClicked: pgStack.push("qrc:/View3.qml")
        }
    }
}

