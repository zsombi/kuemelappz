// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

// Style component for ApplicationWindow
Style {
    // background style object name
    property string backgroundStyle
    // input panel style
    property string inputPanelStyle
    // status bar style
    property string statusBarStyle
    // application layout rotation duration
    property int rotationDuration
    // rotation easing
    property variant rotationEasing
}
