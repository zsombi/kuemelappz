/*
  Default theme (and in general a theme) definition file. The root object properties are accessible
  in the application through THEME variable, e.g. THEME.textSizeSmall, etc.

  As convention, every style name that belongs to a theme should start with "Theme" prefix.
  */

// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Theme {
    name: "default"
    resource: "default_resources"
    // theme global properties
    property int pageFadingDuration: 800
    property int pageFadingEasing: Easing.InOutExpo
    property int pageHeaderLayoutMargins: 6
    property int textSizeSmall: 12
    property int textSizeNormal: 16
    property int textSizeLarge: 22
    property int paddingThin: 2
    property int paddingLow: 4
    property int paddingMedium: 8
    property int paddingLarge: 12
    property string defaultFont: "Nokia Sans"

    property color __headerColor: "#007dA9"
    property color __highlightColor: "#00aacb"

    // theme styles
    /*------------------ApplicationWindow-----------------------------------------*/
    BackgroundStyle {
        name: "ThemeAppBackground"
        image: "qrc:/default/page-background-shape"
        fillMode: Image.Tile
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
    InputPanelStyle {
        name: "ThemeInputPanel"
        fadeInDuration: 175
        fadeOutDuration: 175
        fadeOutDelay: 100
        fadeEasing: Easing.OutCubic
        borderColor: "silver"
        backgroundImageUrl: "qrc:/default/keypad"
    }

    ApplicationWindowStyle {
        name: "ThemeApplicationWindow"
        backgroundStyle: "ThemeAppBackground"
        inputPanelStyle: "ThemeInputPanel"
        rotationDuration: 500
        rotationEasing: Easing.InOutExpo
    }

    CornerFramerStyle {
        name: "ThemeAppRoundCorners"
        sourceLeftTop: "qrc:/default/appwindow-corner-tl"
        sourceRightTop: "qrc:/default/appwindow-corner-tr"
        sourceRightBottom: "qrc:/default/appwindow-corner-br"
        sourceLeftBottom: "qrc:/default/appwindow-corner-bl"
    }
    /*------------------MainMenu-----------------------------------------*/
    PageHeaderStyle {
        name: "ThemeMenuHeader"
        color: "#FF8500"
        headerMargins: 5
        radius: 15
    }
    BackgroundStyle {
        name: "ThemeMenuBackground"
        color: "darkgray"
        radius: 15
    }

    PageLayoutStyle {
        name: "ThemeMenuLayout"
        headerHeight: 45
        headerStyle: "ThemeMenuHeader"
        property string backgroundStyle: "ThemeMenuBackground"
    }
    /*------------------PageLayout-----------------------------------------*/
    PageHeaderStyle {
        name: "ThemePageHeader"
        color: __headerColor
        headerMargins: 5
    }
    PageLayoutStyle {
        name: "ThemePageLayout"
        headerHeight: 55
        headerStyle: "ThemePageHeader"
        transitionDuration: 200
        transitionEasing: Easing.InOutExpo
    }

    /*------------------ToolBar-----------------------------------------*/

    BackgroundStyle {
        name: "ThemeToolbarFrame"
        image: "qrc:/default/gradient"
    }
    ToolButtonStyle {
        name: "PageTabButton"
        type:Style.Normal
        fontFamily: defaultFont
        fontColor: "silver"
        fontPixels: textSizeNormal
    }
    ToolButtonStyle {
        name: "PageTabButton"
        type:Style.Pressed
        imageUrl: "qrc:/default/tab-button"
        fontFamily: defaultFont
        fontColor: __highlightColor
        fontPixels: textSizeNormal
    }
    ToolButtonStyle {
        name: "PageTabButton"
        type:Style.Highlighted
        imageUrl: "qrc:/default/tab-button"
        fontFamily: defaultFont
        fontColor: __highlightColor
        fontPixels: textSizeNormal
    }
    DockStyle {
        name: "ThemeToolbar"
        frameStyle: "ThemeToolbarFrame"
        buttonStyle: "PageTabButton"
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
        name: "ThemeToolButton"
        type:Style.Normal
        imageUrl: "qrc:/default/toolbutton-normal"
        imageBorders: [8,8,8,8]
        fontFamily: defaultFont
        fontColor: "silver"
        fontPixels: textSizeNormal
    }
    ToolButtonStyle {
        name: "ThemeToolButton"
        type:Style.Pressed
        imageUrl: "qrc:/default/toolbutton-pressed"
        imageBorders: [8,8,8,8]
        fontFamily: defaultFont
        fontColor: __highlightColor
        fontPixels: textSizeNormal
    }
    ToolButtonStyle {
        name: "ThemeToolButton"
        type:Style.Highlighted
        imageUrl: "qrc:/default/toolbutton-normal"
        imageBorders: [8,8,8,8]
        fontFamily: defaultFont
        fontColor: __highlightColor
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
