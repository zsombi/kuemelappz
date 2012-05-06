// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Style {
    // delay used before starting fade out animation
    property int fadeOutDelay:0
    // fade in duration
    property int fadeInDuration:0
    // fade out duration
    property int fadeOutDuration:0
    // fade animation easing
    property int fadeEasing:Easing.OutBack
    // border color
    property color borderColor
    // panel's background
    property url backgroundImageUrl
    // key button style
    property string keyButtonStyle
    // control button style
    property string controlButtonStyle
}
