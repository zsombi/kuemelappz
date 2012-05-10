/**
  Button - push button. Suports image and text combined in its layout. The image
  can be either on theleft side of the text or above the text, and either the image
  or the text can be missing.

  The style of the button is specified by the ButtonStyle element. The default style
  object used by the button is "PushButton"

  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

ButtonControl {
    // public API
    // button text
    property string text: ""
    // button image when normal mode
    property url image: ""
    // button image when pressed
    property url imagePressed: image
    // image-text layout
    property int layout: Qt.Horizontal
    // font
    property alias font: buttonText.font

    // style
    styleName: "PushButton"

    id: pushButton
    width: 100
    height: THEME.measures.defButtonHeight
    clip: true
    // handle style type change
    onPressedChanged: styleType = (pressed ? Style.Pressed : (highlighted ? Style.Highlighted : Style.Normal))

    Background {
        id: frame
        anchors.fill: parent
        styleName: pushButton.style.frameStyle
        styleType: pushButton.styleType
    }

    Item {
        id: imageButtonLayout
        property variant imageSize: [0, 0]
        property int offset: -2
        property string layoutState: ""
        //property Item control

        anchors {left: parent.left; top: parent.top; bottom: parent.bottom; right: parent.right}
        Image {
            id: buttonImage
            smooth: true
            source: pushButton.image
            fillMode: Image.PreserveAspectFit
            //anchors.fill: parent
            anchors {left: parent.left; top: parent.top; bottom: parent.bottom; right: parent.right }
            anchors.margins: imageButtonLayout.offset
        }

        Label {
            id: buttonText
            //anchors.fill: parent
            anchors {verticalCenter: parent.verticalCenter; horizontalCenter: parent.horizontalCenter}

            color: pushButton.style.fontColor
            //font.family: pushButton.style.fontFamily
            //font.pixelSize: pushButton.style.fontPixels
            font: pushButton.style.font
            styleColor: Qt.darker(color, 2)
            style: Text.Outline
            //font.bold: true
            //wrapMode: Text.WordWrap
            elide: Text.ElideRight
            smooth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: pushButton.text
        }

        states: [
            State {
                name: "btnPressed"
                extend: imageButtonLayout.layoutState
                when: pushButton.pressed || pushButton.checked
                PropertyChanges {
                    target: buttonText
                    //anchors {topMargin: 6; leftMargin: 6}
                    anchors { verticalCenterOffset: 1; horizontalCenterOffset: 1}
                }
                PropertyChanges {
                    target: buttonImage
                    source: pushButton.imagePressed
                    anchors.margins: imageButtonLayout.offset + 1
                }
            },
            State {
                name: "LRIconCaption"
                when: (layout == Qt.Horizontal) && (image !== "") && (text !== "")
                PropertyChanges {
                    target: imageButtonLayout
                    layoutState: "LRIconCaption"
                }
                AnchorChanges {
                    target: buttonImage
                    anchors.right: undefined
                }
                PropertyChanges {
                    target: buttonImage
                    width: (imageButtonLayout.imageSize[1] <= 0) ? buttonImage.sourceSize.height : imageButtonLayout.imageSize[1]
                }
                PropertyChanges {target: buttonText; anchors.horizontalCenterOffset: buttonImage.width / 2 }
            },
            State {
                name: "TDIconCaption"
                when: (layout == Qt.Vertical) && (image !== "") && (text !== "")
                PropertyChanges {
                    target: imageButtonLayout
                    layoutState: "TDIconCaption"
                }
                AnchorChanges {
                    target: buttonImage
                    anchors.bottom: undefined
                }
                PropertyChanges {
                    target: buttonImage
                    height: (imageButtonLayout.imageSize[0] <= 0) ? buttonImage.sourceSize.width : imageButtonLayout.imageSize[0]
                }
                PropertyChanges {target: buttonText; anchors.verticalCenterOffset: buttonImage.height / 2 }
            }
        ]
    }

}
