import QtQuick 1.0
import com.zesoft.widgetset 1.0

Page {
    title: "Page1"
    anchors.fill: parent
    header.visible: false
    id: page

    FaderItem {
        id: popupTest
        backgroundSource: "qrc:/default/toolbutton-normal"
        //fadeOpacity: 0.8
        //visualParent: page
        fadeStyle: WidgetSet.FadeInOut
        fadeInDuration: 800
        fadeOutDuration: 800
    }

    Column {
        objectName: "columner"
        spacing: 10
        width: parent.width
        //height: parent.height
        LineEdit {
            width: 200
            //height: 25
        }
        Label {
            text: "Page 1"
            font: THEME.largeFont
            color: "white"
        }
        Button {
            id: button
            text: "Quit"
            onClicked: Qt.quit()
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
            onClicked: popupTest.show()
        }
        Button {
            styleName: "NaviBackStyle"
            text: "FadeFromLeft"
            width:140
            height: 50
            onClicked: popupTest.show(WidgetSet.FadeLeft)
        }
        Button {
            styleName: "NaviBackStyle"
            text: "FadeFromTop"
            width:140
            height: 50
            onClicked: popupTest.show(WidgetSet.FadeTop)
        }
        Button {
            styleName: "NaviBackStyle"
            text: "FadeFromRight"
            width:140
            height: 50
            onClicked: popupTest.show(WidgetSet.FadeRight)
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
            onClicked: popupTest.show(WidgetSet.FadeBottom)
        }
    }
}
