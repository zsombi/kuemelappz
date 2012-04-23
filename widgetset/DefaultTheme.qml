// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/*
  Default theme (and in general a theme) definition file. The root object properties are accessible
  in the application through UI variable, e.g. UI.appRotationDuration, etc
  */

Theme {
    name: "default"
    resource: "default_resources"
    // private properties
    property int appRotationDuration: 500
    property int pageFadingDuration: 250
    property int textSizeSmall: 12
    property int textSizeNormal: 16
    property int textSizeLarge: 22
    property int paddingLow: 4
    property int paddingMedium: 8
    property int paddingLarge: 12
    property string defaultFont: "Nokia Sans"
    property url appBackgroundImage: "qrc:/default/page-background-shape"
    property url pageBackgroundImage//: "qrc:/default/page-background-diagonal"
    // theme styles
    IndicatorStyle {
        busyIndicatorUrl: "qrc:/default/busy"
    }
    StatusBarStyle {
        fillColor: "black"
        fontFamily: defaultFont
        fontColor: "white"
        fontPixels: textSizeSmall
        fontWeight: Font.Bold
    }
    CornerFramerStyle {
        name: "AppRoundCorners"
        sourceLeftTop: "qrc:/default/appwindow-corner-tl"
        sourceRightTop: "qrc:/default/appwindow-corner-tr"
        sourceRightBottom: "qrc:/default/appwindow-corner-br"
        sourceLeftBottom: "qrc:/default/appwindow-corner-bl"
    }

    DockStyle {
        name: "DockStyle"
        imageUrl: "qrc:/default/dock-background"
        imageBorders: [2,2,2,2]
    }

    DockStyle {
        name: "ToolBar"
        imageUrl: "qrc:/default/gradient"
        imageBorders: [0,0,0,0]
    }

    DockStyle {
        name: "Gradient"
        frameColor: "#666666"
        frameWidth: 1
        fillColor: "#333333"
        gradientSteps: [0,0.1,0.4,1]
        gradientTones: [1.9,1.5,1.2,0.8]
    }

    ToolButtonStyle {
        name: "ToolButton"
        set:Style.Normal
        fontFamily: defaultFont
        fontColor: "silver"
        fontPixels: textSizeNormal
    }
    ToolButtonStyle {
        name: "ToolButton"
        set:Style.Pressed
        imageUrl: "qrc:/default/tool-button"
        fontFamily: defaultFont
        fontColor: "#00aacb"
        fontPixels: textSizeNormal
    }
    ToolButtonStyle {
        name: "ToolButton"
        set:Style.Highlighted
        imageUrl: "qrc:/default/tool-button"
        fontFamily: defaultFont
        fontColor: "#00aacb"
        fontPixels: textSizeNormal
    }

    ButtonStyle {
        set: Style.Normal
        imageUrl: "qrc:/default/pushbutton-normal"
        imageBorders: [6,6,6,6]
        fontFamily: "Arial"
        fontColor: "white"
        fontPixels: textSizeNormal
    }
    ButtonStyle {
        set: Style.Pressed
        imageUrl: "qrc:/default/pushbutton-pressed"
        imageBorders: [6,6,6,6]
        fontFamily: "Arial"
        fontColor: "lightgray"
        fontPixels: textSizeNormal
    }

    ButtonStyle {
        name: "NaviBackStyle"
        set: Style.Normal
        imageUrl: "qrc:/default/navi-back"
        imageBorders: [14,5,5,5]
        fontFamily: "Nokia Sans"
        fontColor: "white"
    }
    ButtonStyle {
        name: "NaviBackStyle"
        set: Style.Pressed
        imageUrl: "qrc:/default/navi-back-pressed"
        imageBorders: [14,5,5,5]
        fontFamily: "Nokia Sans"
        fontColor: "white"
    }

    TextEditStyle {
        name: "LineEdit"
        imageUrl: "qrc:/default/editline-background"
        imageBorders: [10,5,10,5]
        hintFontFamily: defaultFont
        hintFontPixels: textSizeNormal
        hintFontItalic: true
        hintFontColor: "#a2a2a2"
        editorFontFamily: defaultFont
        editorFontPixels: textSizeNormal
    }

    LabelStyle {
        fontFamily: "Nokia Sans"
        fontColor: "silver"
        fontPixels: textSizeNormal
        fontWeight: Font.DemiBold
    }

    ScrollBarStyle {
        frameColor: "transparent"
        barColor: "#9F9F9F"
        frameOpacity: 0
        fadeInDuration: 100
        fadeOutDuration: 500
        fadeOutDelay: 300
    }
}
