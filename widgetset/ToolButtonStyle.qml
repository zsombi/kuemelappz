// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Style {
    // tool button frame image
    property url imageUrl
    // frame image borders (array of 4 elements, left-top,right-bottom)
    property variant imageBorders
    // font color
    property color fontColor
    // font family
    property string fontFamily
    // font pixels
    property int fontPixels
}
