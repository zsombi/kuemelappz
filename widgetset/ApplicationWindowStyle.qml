// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

// Style component for ApplicationWindow
Style {
    name: "ApplicationWindow"

    // style object to control layout
    property string pageLayoutStyle
    // background style object name
    property string backgroundStyle
    // header style object name
    property string headerStyle
    // application layout rotation duration
    property int rotationDuration
    // rotation easing
    property variant rotationEasing
}
