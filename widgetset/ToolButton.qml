import QtQuick 1.1
import "." 1.0

ButtonControl {
    radio: false; checkable: true
    property string image: ""
    property string imageActive: image
    property string text: ""
    property variant action: text
    // style
    styleName: "ToolButton"

    id: toolButton
    height: parent.height
    width: height

    onPressedChanged: {
        if (pressed)
            styleType = Style.Pressed
    }
    onCheckedChanged: {
        if (checked && radio)
            styleType = Style.Pressed
        else
            styleType = Style.Normal
    }

    objectName: text

    Image {
        anchors.fill: parent
        source: toolButton.style.imageUrl
    }
    Item {
        id: buttonData
        anchors.fill: parent

        Image {
            id: toolImage
            height: width
            smooth: true
            anchors.right: parent.right
            anchors.rightMargin: 14
            anchors.left: parent.left
            anchors.leftMargin: 14
            anchors.top: parent.top
            anchors.topMargin: 3
            source: image
        }

        Text {
            id: buttonText
            y: 37
            height: paintedHeight
            color: toolButton.style.fontColor
            text: toolButton.text
            style: Text.Normal
            smooth: true
            elide: Text.ElideMiddle
            horizontalAlignment: Text.AlignHCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.left: parent.left
            anchors.leftMargin: 2
            verticalAlignment: Text.AlignBottom
            clip: true
            font.family: toolButton.style.fontFamily
            font.pixelSize: toolButton.style.fontPixels
        }

    states: [
        State {
            name: "btnActive"
            when: toolButton.checked || toolButton.highlighted
            PropertyChanges { target: toolImage; source: toolButton.imageActive }
        },
        State {
            name: "btnPressed"
            when: toolButton.pressed
            PropertyChanges { target: buttonData; scale: 0.95 }
            PropertyChanges { target: toolImage; source: toolButton.imageActive }
        }
    ]
    }
}
