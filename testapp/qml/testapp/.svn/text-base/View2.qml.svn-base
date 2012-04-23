import QtQuick 1.0
import com.zesoft.widgetset 1.0

Page {
    title: "Page1"
    anchors.fill: parent

    FaderItem {
        id: popupTest
        backgroundSource: "qrc:/default/toolbutton-normal"
        //fadeOpacity: 0.8
        visualParent: parent
        fadeStyle: WidgetSet.FadeInOut
    }

    Scrollable {
        anchors.fill: parent
        Column {
            objectName: "columner"
            spacing: 10
            width: parent.width
            LineEdit {
                width: 200
                //height: 25
            }
            Label {
                text: "Page 1"
                font.pixelSize: UI.textSizeLarge
                color: "white"
            }
            Button {
                id: button
                text: "Quit"
                onClicked: {console.debug("QUIT"); Qt.quit()}
            }
            LineEdit {
                width: 200
                //height: 25
            }
            Button {
                styleName: "NaviBackStyle"
                text: "FadeInOut"
                width:140
                height: 50
                onClicked: popupTest.open()
            }
            Button {
                styleName: "NaviBackStyle"
                text: "FadeFromLeft"
                width:140
                height: 50
                onClicked: popupTest.open(WidgetSet.FadeFromLeft)
            }
            Button {
                styleName: "NaviBackStyle"
                text: "FadeFromTop"
                width:140
                height: 50
                onClicked: popupTest.open(WidgetSet.FadeFromTop)
            }
            Button {
                styleName: "NaviBackStyle"
                text: "FadeFromRight"
                width:140
                height: 50
                onClicked: popupTest.open(WidgetSet.FadeFromRight)
            }
            LineEdit {
                width: 170
                //height: 25
            }

            Button {
                styleName: "NaviBackStyle"
                text: "FadeFromBottom"
                width:140
                height: 50
                onClicked: popupTest.open(WidgetSet.FadeFromBottom)
            }

        }
    }
}
