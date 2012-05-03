import QtQuick 1.0
import com.zesoft.widgetset 1.0

PageStack {
    property string title: "PageStack"
    property url image
    property url imageActive
    id: pgStack
    anchors.fill: parent
    width: parent.width
    height: parent.width

    Component {
        id: pg1
        Page {
            width: pgStack.width
            height: pgStack.width
            headerLayout: PageHeader {
                Button {
                    text: "Quit"
                    onClicked: Qt.quit()
                }
            }
            Column {
                spacing: 10
                Label {
                    color: "white"
                    text: "Page #" + pgStack.depth
                    font.pixelSize: THEME.textSizeLarge
                }
                Button {
                    text: "Back"
                    onClicked: pgStack.back()
                }
                Button {
                    text: "Next"
                    onClicked: pgStack.push(pg1)
                }
            }
        }
    }

    Component.onCompleted: pgStack.push(pg1)//"qrc:/View3.qml")
}
/*
Page {
    objectName: "Page1"
    property color pcolor: style.styleProperty("Panel.panelColor")
    panel: [
        Panel{
            panelColor: "blue"
        },

        Panel{
        ButtonFlow {
            width: 200
            height: 35
            anchors.centerIn: parent
            radioType: true
            PushButton {
                caption: "A"
            }
            PushButton {
                caption: "B"
            }
            PushButton {
                caption: "C"
            }
            PushButton {
                caption: "D"
            }
        }
        }
    ]

    Column {
        spacing: 10
        width: parent.width
        Text {
            text: "Page 1"
            color: "white"
            font.bold: true
            font.pixelSize: 20
        }

        PushButton {
            fillColor: "lightblue"
            caption: "Pushbutton"
            image: "image://icon/CheckMark?type=qrc&source=default&alpha=tan"
            imageSize: [25, 25]
            layout: Qt.Vertical
        }
        CheckButton {
            width: parent.width
        }
        CheckButton {
            width: parent.width
        }
        CheckButton {
            width: parent.width
        }
        CheckButton {
            width: parent.width
        }
        Icon {
            source: "image://icon/BusyIndicator?type=qrc&source=default"
            color: "#ABABAB"
            width: parent.width
            height: 100
        }

        Progressbar {
            width: parent.width
            progress: 0.3
        }

        Spinner {
            id: mspin
            width: 30
            height: 180
            delegate: Text {id: delg; font.pixelSize: 20; text: ((m < 10) ? "0" : "") + m; height: 20; color: "white" }

            itemHeight: 20

            flickDeceleration: 100
            model: ListModel {
                id: minModel
            }
            focus: true

            Component.onCompleted: {
                var i;
                for (i = 0; i < 60; i++) minModel.append({m: i});
            }

            //onCurrentIndexChanged: console.log("spinner item: " + minModel.get(currentIndex).m)

        }
    }
}
/*
Item {
    id: page
    property bool enableScrolling: true
    default property alias content: pageContent.data

    anchors.fill: parent
    clip: true

    Flickable {
        id: scrollable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        interactive: enableScrolling && (pageContent.height > height)
        contentWidth: width
        contentHeight: pageContent.height

        Item {
            id: pageContent
            width: parent.width
            height: childrenRect.height
        }
        z: 0
    }
    Rectangle {
        z: -1
        id: background
        anchors.fill: parent
        color: styles.color("PageBackground")
    }
}
*/
