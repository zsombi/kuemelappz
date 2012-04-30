// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

// background style
Style {
    // background image
    property url image
    //image fill mode
    property int fillMode
    // image borders, array of min. 4 elements: left, top, right and bottom
    property variant imageBorders
    // color for solid and gradient fills
    property color color
    // radius for the rectangle used for gradient
    property real radius
    // gradient stops; only the first 4 are taken into use!
    property variant gradientStops
    // gradient tones for the fill color; only the first 3 elements are taken into use
    property variant gradientTones
}
