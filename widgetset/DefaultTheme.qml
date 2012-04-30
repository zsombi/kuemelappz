// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/*
  Default theme (and in general a theme) definition file. The root object properties are accessible
  in the application through THEME variable, e.g. THEME.textSizeSmall, etc
  */

Theme {
    name: "default"
    resource: "default_resources"
    // theme global properties
    property int pageFadingDuration: 800
    property int pageFadingEasing: Easing.InOutExpo
    property int textSizeSmall: 12
    property int textSizeNormal: 16
    property int textSizeLarge: 22
    property int paddingThin: 2
    property int paddingLow: 4
    property int paddingMedium: 8
    property int paddingLarge: 12
    property string defaultFont: "Nokia Sans"

    // theme styles
    /*------------------ApplicationWindow-----------------------------------------*/
    BackgroundStyle {
        name: "Header"
        color: "#007dc9"
    }
    BackgroundStyle {
        name: "AppBackground"
        image: "qrc:/default/page-background-shape"
        fillMode: Image.Tile
    }
    PageLayoutStyle {
        name: "PageLayout"
        headerHeight: 55
        transitionDuration: 200
        transitionEasing: Easing.InOutExpo
    }
    IndicatorStyle {
        busyIndicatorUrl: "qrc:/default/busy"
    }
    StatusBarStyle {
        height: 15
        fillColor: "black"
        fontFamily: defaultFont
        fontColor: "white"
        fontPixels: textSizeSmall
        fontWeight: Font.Bold
        transitionEasing: Easing.InOutExpo
        transitionDuration: 200
    }
    ApplicationWindowStyle {
        pageLayoutStyle: "PageLayout"
        headerStyle: "Header"
        backgroundStyle: "AppBackground"
        rotationDuration: 500
        rotationEasing: Easing.InOutExpo
    }

    CornerFramerStyle {
        name: "AppRoundCorners"
        sourceLeftTop: "qrc:/default/appwindow-corner-tl"
        sourceRightTop: "qrc:/default/appwindow-corner-tr"
        sourceRightBottom: "qrc:/default/appwindow-corner-br"
        sourceLeftBottom: "qrc:/default/appwindow-corner-bl"
    }

    /*------------------ToolBar-----------------------------------------*/

    BackgroundStyle {
        name: "toolbarFrame"
        image: "qrc:/default/gradient"
    }

    DockStyle {
        name: "ToolBar"
        frameStyle: "toolbarFrame"
        buttonSpacing: 3
    }

    /*-----------------------------------------------------------*/
    BackgroundStyle {
        name: "dockFrame"
        image: "qrc:/default/dock-background"
        imageBorders: [2,2,2,2]
    }

    DockStyle {
        name: "DockStyle"
        frameStyle: "dockFrame"
        buttonSpacing: 3
    }

    ToolButtonStyle {
        name: "ToolButton"
        type:Style.Normal
        fontFamily: defaultFont
        fontColor: "silver"
        fontPixels: textSizeNormal
    }
    ToolButtonStyle {
        name: "ToolButton"
        type:Style.Pressed
        imageUrl: "qrc:/default/tool-button"
        fontFamily: defaultFont
        fontColor: "#00aacb"
        fontPixels: textSizeNormal
    }
    ToolButtonStyle {
        name: "ToolButton"
        type:Style.Highlighted
        imageUrl: "qrc:/default/tool-button"
        fontFamily: defaultFont
        fontColor: "#00aacb"
        fontPixels: textSizeNormal
    }

    /*-------------------PushButton----------------------------------------*/
    BackgroundStyle {
        name: "btnFrame"
        image: "qrc:/default/pushbutton-normal"
        imageBorders: [6,6,6,6]
    }
    BackgroundStyle {
        name: "btnFrame"
        type: Style.Pressed
        image: "qrc:/default/pushbutton-pressed"
        imageBorders: [6,6,6,6]
    }
    ButtonStyle {
        type: Style.Normal
        frameStyle: "btnFrame"
        fontFamily: "Arial"
        fontColor: "white"
        fontPixels: textSizeNormal
    }
    ButtonStyle {
        type: Style.Pressed
        frameStyle: "btnFrame"
        fontFamily: "Arial"
        fontColor: "lightgray"
        fontPixels: textSizeNormal
    }

    /*----------------NaviBack-------------------------------------------*/
    BackgroundStyle {
        name: "btnBackFrame"
        image: "qrc:/default/navi-back"
        imageBorders: [14,5,5,5]
    }
    BackgroundStyle {
        name: "btnBackFrame"
        type: Style.Pressed
        image: "qrc:/default/navi-back-pressed"
        imageBorders: [14,5,5,5]
    }
    ButtonStyle {
        name: "NaviBackStyle"
        frameStyle: "btnBackFrame"
        fontFamily: "Nokia Sans"
        fontColor: "white"
        fontPixels: textSizeNormal
    }
    ButtonStyle {
        name: "NaviBackStyle"
        frameStyle: "btnBackFrame"
        fontFamily: "Nokia Sans"
        fontColor: "white"
        fontPixels: textSizeNormal
    }

    /*----------------LineEdit-------------------------------------------*/
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

    /*----------------Label-------------------------------------------*/
    LabelStyle {
        fontFamily: "Nokia Sans"
        fontColor: "silver"
        fontPixels: textSizeNormal
        fontWeight: Font.DemiBold
    }

    /*----------------Scrollbar-------------------------------------------*/
    ScrollBarStyle {
        frameColor: "transparent"
        barColor: "#9F9F9F"
        frameOpacity: 0
        fadeInDuration: 100
        fadeOutDuration: 500
        fadeOutDelay: 300
    }

    /*-----------------------------------------------------------*/
}
