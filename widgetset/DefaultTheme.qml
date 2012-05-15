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
    property int panelFadingEasing: Easing.InOutExpo
    property int panelFadingDuration: 200
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
        font.pixelSize: THEME.sizes.smallFontSize
        font.weight: Font.Light
    }
    Text {
        id: smallBoldFont
        font.family: "Nokia Sans"
        font.pixelSize: THEME.sizes.smallFontSize
        font.weight: Font.Bold
    }
    Text {
        id: normalFontItem
        font.family: "Nokia Sans"
        font.pixelSize: THEME.sizes.normalFontSize
        font.weight: Font.Normal
    }
    Text {
        id: normalItalicFont
        font.family: "Nokia Sans"
        font.pixelSize: THEME.sizes.normalFontSize
        font.weight: Font.Normal
        font.italic: true
    }
    Text {
        id: largeFontItem
        font.family: "Nokia Sans"
        font.pixelSize: THEME.sizes.largeFontSize
        font.weight: Font.DemiBold
    }

    /*-------------------Portrait/Landscape measurements-------------------*/

    /*-------------------Normal dislay High density------------------------*/
    Measurements {
        name: "Normal/High/Portrait"
        smallFontSize: 11
        normalFontSize: 14
        largeFontSize: 22
        statusBarHeight: 18
        headerHeight: 55
        pageTabHeight: 60
        inputPanelHeight: 180
        defButtonHeight: 40
        defLineEditHeight:30
        scrollDecoratorThickness: 8
        dockHeight: 50
        spacingSmall:1
        spacingMedium:3
        spacingNormal:5
        spacingLarge:9
    }
    Measurements {
        name: "Normal/High/Landscape"
        smallFontSize: 11
        normalFontSize: 14
        largeFontSize: 22
        statusBarHeight: 18
        headerHeight: 50
        pageTabHeight: 55
        inputPanelHeight: 140
        defButtonHeight: 40
        defLineEditHeight: 30
        scrollDecoratorThickness: 8
        dockHeight: 50
        spacingSmall:1
        spacingMedium:3
        spacingNormal:5
        spacingLarge:9
    }

    /*-------------------Normal dislay ExtraHigh density-------------------*/
    Measurements {
        name: "Normal/ExtraHigh/Portrait"
        smallFontSize: 11
        normalFontSize: 16
        largeFontSize: 24
        statusBarHeight: 18
        headerHeight: 60
        pageTabHeight: 65
        inputPanelHeight: 240
        defButtonHeight: 50
        defLineEditHeight:45
        scrollDecoratorThickness: 10
        dockHeight: 65
        spacingSmall:1
        spacingMedium:3
        spacingNormal:5
        spacingLarge:9
    }
    Measurements {
        name: "Normal/ExtraHigh/Landscape"
        smallFontSize: 11
        normalFontSize: 16
        largeFontSize: 42
        statusBarHeight: 18
        headerHeight: 55
        pageTabHeight: 60
        inputPanelHeight: 190
        defButtonHeight: 50
        defLineEditHeight: 45
        scrollDecoratorThickness: 10
        dockHeight: 60
        spacingSmall:1
        spacingMedium:3
        spacingNormal:5
        spacingLarge:9
    }
    /*-------------------Default measurements------------------------------*/
    Measurements {
        name: "Portrait"
        smallFontSize: 11
        normalFontSize: 14
        largeFontSize: 22
        statusBarHeight: 18
        headerHeight: 55
        pageTabHeight: 60
        inputPanelHeight: 180
        defButtonHeight: 45
        defLineEditHeight:30
        scrollDecoratorThickness: 8
        dockHeight: 50
        spacingSmall:1
        spacingMedium:3
        spacingNormal:5
        spacingLarge:9
    }
    Measurements {
        name: "Landscape"
        smallFontSize: 11
        normalFontSize: 14
        largeFontSize: 22
        statusBarHeight: 18
        headerHeight: 50
        pageTabHeight: 55
        inputPanelHeight: 160
        defButtonHeight: 45
        defLineEditHeight: 30
        scrollDecoratorThickness: 8
        dockHeight: 50
        spacingSmall:1
        spacingMedium:3
        spacingNormal:5
        spacingLarge:9
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
        controlButtonStyle: "ThemePushButton"
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
        rotationStartPause: 200
        scalingDuration: 100
        rotationDuration: 500
        rotationEasing: Easing.InOutExpo
        animationScaling: 0.8
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

    /*------------------PageLayout-----------------------------------------*/
    PageHeaderStyle {
        name: "ThemePageHeader"
        color: __headerColor
        headerMargins: 5
    }

    /*------------------ToolBar-----------------------------------------*/

    // style for the dock/toolbar
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

    /*-----------------------------------------------------------*/
    BackgroundStyle {
        name: "dockFrame"
        image: "qrc:/default/dock-background"
        imageBorders: [2,2,2,2]
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
        name: "ThemePushButton"
        type: Style.Normal
        frameStyle: "btnFrame"
        font: normalFontItem.font
        fontColor: "white"
    }
    ButtonStyle {
        name: "ThemePushButton"
        type: Style.Pressed
        frameStyle: "btnFrame"
        font: normalFontItem.font
        fontColor: "lightgray"
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
        fontColor: "white"
    }
    ButtonStyle {
        name: "NaviBackStyle"
        frameStyle: "btnBackFrame"
        font: normalFontItem.font
        fontColor: "white"
    }

    /*----------------LineEdit-------------------------------------------*/
    TextEditStyle {
        name: "ThemeLineEdit"
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
        name: "ThemeScrollBar"
        frameColor: "transparent"
        barColor: "#9F9F9F"
        frameOpacity: 0
        fadeInDuration: 100
        fadeOutDuration: 500
        fadeOutDelay: 300
    }

    /*-----------------------------------------------------------*/
}
