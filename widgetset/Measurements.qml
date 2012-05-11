/**
  Measurements - style object holding orientation specific measurements. Themes
  should declare one for Portrait and one for Landscape. The values are expected
  in millimeters
*/
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Style {
    property real smallFontSize: 0
    property real normalFontSize: 0
    property real largeFontSize: 0
    property real statusBarHeight:0
    property real headerHeight:0
    property real pageTabHeight:0
    property real inputPanelHeight:0
    property real defButtonHeight:0
    property real defLineEditHeight:0
    property real dockHeight: 0
    property real spacingSmall:0
    property real spacingMedium:0
    property real spacingNormal:0
    property real spacingLarge:0
}
