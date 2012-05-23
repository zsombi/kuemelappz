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
    // font property
    property alias font: buttonText.font
    // style
    styleName: "ThemeToolButton"

    id: toolButton
    //focus: true
    focusType: StyledItem.Focusable
    radio: false; checkable: true
    //onActivated: checked = true
    //onDeactivated: checked = false

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

    objectName: "ToolButton/"+text

    Background {
        anchors.fill: parent
        styleName: toolButton.style.frameStyle
        styleType: toolButton.styleType
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
            font: toolButton.style.font
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
