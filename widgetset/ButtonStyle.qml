// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/*
  Template theme style for PushButtons. Themes should declare
  several items with different set property, for normal, pressed,
  dimmed and highlighted
  */

Style {
    name: "PushButton"

    // button rame style as defined in BackgroundStyle
    property string frameStyle
    // fi[ont used in text
    property string fontFamily
    // font color
    property color fontColor
    // font size in pixels
    property int fontPixels
}
