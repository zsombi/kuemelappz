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
    property int paddingThin: 2
    property int paddingLow: 4
    property int paddingMedium: 8
    property int paddingLarge: 12
    property alias smallFont: smallFontItem.font
    property alias normalFont: normalFontItem.font
    property alias largeFont: largeFontItem.font

    property color __headerColor: "#007dA9"
    property color __highlightColor: "#00aacb"
    /*--------------------FONTS*/
    Text {
        id: smallFontItem
        font.family: "Nokia Sans"
        font.pixelSize: 12
        font.weight: Font.Light
    }
    Text {
        id: smallBoldFont
        font.family: "Nokia Sans"
        font.pixelSize: 12
        font.weight: Font.Bold
    }
    Text {
        id: normalFontItem
        font.family: "Nokia Sans"
        font.pixelSize: 16
        font.weight: Font.Normal
    }
    Text {
        id: normalItalicFont
        font.family: "Nokia Sans"
        font.pixelSize: 16
        font.weight: Font.Normal
        font.italic: true
    }
    Text {
        id: largeFontItem
        font.family: "Nokia Sans"
        font.pixelSize: 22
        font.weight: Font.DemiBold
    }

    // theme styles
    /*-------------------InputPanel----------------------------------------*/
    BackgroundStyle {
        name: "keyBtnFrame"
        type: Style.Normal
        types: [Style.Normal, Style.Pressed]
        image: "qrc:/default/toolbutton-normal"
        imageBorders: [6,6,6,6]
    }
    BackgroundStyle {
        name: "keyBtnFrame"
        type: Style.Pressed
        image: "qrc:/default/toolbutton-pressed"
        imageBorders: [6,6,6,6]
    }
    ButtonStyle {
        name: "keyButton"
        type: Style.Normal
        font: normalFontItem.font
        frameStyle: "toolBtnFrame"
        fontColor: "white"
    }
    ButtonStyle {
        name: "keyButton"
        type: Style.Pressed
        frameStyle: "toolBtnFrame"
        font: normalFontItem.font
        fontColor: "lightgray"
    }
    InputPanelStyle {
        name: "ThemeInputPanel"
        fadeInDuration: 175
        fadeOutDuration: 175
        fadeOutDelay: 100
        fadeEasing: Easing.OutCubic
        borderColor: "silver"
        backgroundImageUrl: "qrc:/default/keypad"
        keyButtonStyle: "keyButton"
        controlButtonStyle: "PushButton"
    }
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
        height: 17
        name: "statusBar"
        fillColor: "black"
        fontColor: "white"
        font: smallBoldFont.font
        transitionEasing: Easing.InOutExpo
        transitionDuration: 200
    }

    ApplicationWindowStyle {
        name: "ThemeApplicationWindow"
        backgroundStyle: "ThemeAppBackground"
        inputPanelStyle: "ThemeInputPanel"
        statusBarStyle: "statusBar"
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
    BackgroundStyle {
        name: "pgTabFrame"
        type:Style.Normal
        color: "transparent"
    }
    BackgroundStyle {
        name: "pgTabFrame"
        type:Style.Pressed
        image: "qrc:/default/tab-button"
        //imageBorders: [8,8,8,8]
    }
    BackgroundStyle {
        name: "pgTabFrame"
        type:Style.Highlighted
        image: "qrc:/default/tab-button"
        //imageBorders: [8,8,8,8]
    }
    ButtonStyle {
        name: "PageTabButton"
        type:Style.Normal
        frameStyle: "pgTabFrame"
        font: normalFontItem.font
        fontColor: "silver"
    }
    ButtonStyle {
        name: "PageTabButton"
        type:Style.Pressed
        frameStyle: "pgTabFrame"
        font: normalFontItem.font
        fontColor: __highlightColor
    }
    ButtonStyle {
        name: "PageTabButton"
        type:Style.Highlighted
        frameStyle: "pgTabFrame"
        font: normalFontItem.font
        fontColor: __highlightColor
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

    /*-------------------ToolButton----------------------------------------*/
    BackgroundStyle {
        name: "toolBtnFrame"
        type:Style.Normal
        image: "qrc:/default/toolbutton-normal"
        imageBorders: [8,8,8,8]
    }
    BackgroundStyle {
        name: "toolBtnFrame"
        type:Style.Pressed
        image: "qrc:/default/toolbutton-pressed"
        imageBorders: [8,8,8,8]
    }
    ButtonStyle {
        name: "ThemeToolButton"
        type:Style.Normal
        frameStyle: "toolBtnFrame"
        font: normalFontItem.font
        fontColor: "silver"
    }
    ButtonStyle {
        name: "ThemeToolButton"
        type:Style.Pressed
        frameStyle: "toolBtnFrame"
        font: normalFontItem.font
        fontColor: "silver"
    }
    ButtonStyle {
        name: "ThemeToolButton"
        type:Style.Highlighted
        frameStyle: "toolBtnFrame"
        font: normalFontItem.font
        fontColor: "silver"
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
        name: "PushButton"
        type: Style.Normal
        frameStyle: "btnFrame"
        font: normalFontItem.font
        //fontFamily: "Arial"
        fontColor: "white"
        //fontPixels: textSizeNormal
    }
    ButtonStyle {
        name: "PushButton"
        type: Style.Pressed
        frameStyle: "btnFrame"
        font: normalFontItem.font
        //fontFamily: "Arial"
        fontColor: "lightgray"
        //fontPixels: textSizeNormal
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
        font: normalFontItem.font
        //fontFamily: "Nokia Sans"
        fontColor: "white"
        //fontPixels: textSizeNormal
    }
    ButtonStyle {
        name: "NaviBackStyle"
        frameStyle: "btnBackFrame"
        font: normalFontItem.font
        //fontFamily: "Nokia Sans"
        fontColor: "white"
        //fontPixels: textSizeNormal
    }

    /*----------------LineEdit-------------------------------------------*/
    TextEditStyle {
        name: "LineEdit"
        image: "qrc:/default/editline-background"
        imageBorders: [10,5,10,5]
        hintFont: normalItalicFont.font
        editorFont: normalFontItem.font
        hintColor: "#a2a2a2"
    }

    /*----------------Label-------------------------------------------*/
    LabelStyle {
        font: normalFont
        fontColor: "white"
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
