/**
  KeyItem - Item handling key presses in input panel
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    id: keyItem
    width: 100
    height: 62

    // key code set
    property string key: ''
    // signal triggered when key is used
    signal clicked()

    Background {
        id: frame
        anchors.fill: parent
        styleName: keyItem.style.frameStyle
        styleType: keyItem.styleType
    }

    Text {
        id: label
        anchors.fill: parent
        text: keyItem.key
        verticalAlignment: Text.Center
        horizontalAlignment: Text.Center
        font.family: keyItem.style.fontFamily
        color: keyItem.style.fontColor
        font.pixelSize: keyItem.style.fontPixels
    }

    MouseArea {
        id: control
        anchors.fill: parent
        onPressed: keyItem.styleType = Style.Pressed
        onReleased: keyItem.styleType = Style.Normal
        onClicked: keyItem.clicked()
    }
}
