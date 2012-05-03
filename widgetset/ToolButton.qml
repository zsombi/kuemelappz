/**
  ToolButton - button, which is checkable.
  */

import QtQuick 1.1
import "." 1.0

ButtonControl {
    // tool image
    property string image: ""
    // tool image when active
    property string imageActive: image
    //tool text
    property string text: ""
    // action assigned when activated
    property variant action: text
    // style
    styleName: "ThemeToolButton"

    //onStyleChanged: console.debug("ToolButton #"+buttonId+" style: " + styleName + ", type: " + styleType + ": style: " + style)

    id: toolButton
    height: parent.height
    width: height
    radio: false; checkable: true

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

    BorderImage {
        anchors.fill: parent
        source: toolButton.style.imageUrl
        border {
            left: (toolButton.style.imageBorders != undefined) ? toolButton.style.imageBorders[0] : 0
            top: (toolButton.style.imageBorders != undefined) ? toolButton.style.imageBorders[1] : 0
            right: (toolButton.style.imageBorders != undefined) ? toolButton.style.imageBorders[2] : 0
            bottom: (toolButton.style.imageBorders != undefined) ? toolButton.style.imageBorders[3] : 0
        }
    }
    Item {
        id: buttonData
        anchors.fill: parent

        Image {
            id: toolImage
            height: parent.height - buttonText.paintedHeight - 7
            width: height
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 3
            source: toolButton.image
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
            anchors.bottomMargin: 2
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
                when: toolButton.checked || toolButton.highlighted ||  toolButton.pressed
                PropertyChanges { target: toolImage; source: toolButton.imageActive }
            }
        ]
    }
}
