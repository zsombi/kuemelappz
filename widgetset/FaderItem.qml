/**
  FaderItem - base item for popup type components. Styling comes from the inherited
  components.

  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

Rectangle {
    id: faderItem
    /*** @public interface
      */
    // specifies the parent to be used when fader gets visible
    property Item visualParent: null
    // indicates whether the fader is visible or not
    property bool fadedIn: false
    // property driving automatic hide (fadeOut) when inactive area is clicked
    property bool hideOnInactiveAreaClicked: true
    // fader margins, calculated from the visibleParent
    property int faderMarginLeft: -1
    property int faderMarginTop: -1
    property int faderMarginRight: -1
    property int faderMarginBottom: -1

    // Stylable properties
    // specifies the opacity value used when component is faded in
    property double opacityWhenFadeIn: 1.0
    // deprecate: background source image
    property url backgroundSource: ""
    // delay used before fadeOut animation starts
    property int fadeOutDelay: 175
    // duration of fadeIn animation
    property int fadeInDuration: 175
    // duration of fadeOut animation
    property int fadeOutDuration: 175
    // Specifies how to fade in/out, from which direction (see WidgetSet.FaderStyle); default is FadeInOut
    property int fadeStyle: WidgetSet.FadeInOut

    // signal triggered when inactive area clicked
    signal inactiveAreaClicked()

    // shows the fader (fadeIn); the faderStyle can be specified separately so the same fader component
    // can be used to fade in and out in different way
    function show(withFadeStyle)
    {
        if (withFadeStyle == undefined)
            fadeStyle = WidgetSet.FadeInOut
        else
            fadeStyle = withFadeStyle
        state = 'visible'
    }
    // hides the fader
    function hide()
    {
        state = ''
    }

    /*** @internals
      */
    // automatic close
    onInactiveAreaClicked: if (hideOnInactiveAreaClicked) hide()
    color: backgroundSource != "" ? "transparent" : Qt.rgba(0.05, 0.05, 0.05, privates.alpha)
    z: Number.MAX_VALUE
    anchors {
        margins: -1 // enlarge it to avoid artefacts appearing upon orientation change
        leftMargin: {
            if (fadeStyle == WidgetSet.FadeLeft)
                return -1
            return faderMarginLeft
        }
        left: {
            if (fadeStyle == WidgetSet.FadeLeft)
                return undefined
            else if (fadeStyle == WidgetSet.FadeRight)
                return parent.right
            return parent.left
        }

        topMargin: {
            if (fadeStyle == WidgetSet.FadeTop)
                return -1
            return faderMarginTop
        }
        top: {
            if (fadeStyle == WidgetSet.FadeTop)
                return undefined
            else if (fadeStyle == WidgetSet.FadeBottom)
                return parent.bottom
            return parent.top
        }

        rightMargin: {
            if (fadeStyle == WidgetSet.FadeRight)
                return -1
            return faderMarginRight
        }
        right: {
            if (fadeStyle == WidgetSet.FadeRight)
                return undefined
            else if (fadeStyle == WidgetSet.FadeLeft)
                return parent.left
            return parent.right
        }

        bottomMargin: {
            if (fadeStyle == WidgetSet.FadeBottom)
                return -1
            return faderMarginBottom
        }
        bottom: {
            if (fadeStyle == WidgetSet.FadeBottom)
                return undefined
            else if (fadeStyle == WidgetSet.FadeTop)
                return parent.top
            return parent.bottom
        }
    }

    QtObject {
      id: privates
      property double alpha: 0.0
      property Item originalParent: faderItem.parent
      onAlphaChanged: {
          if (backgroundSource && privates.alpha && backgroundLoader.sourceComponent == undefined) {
            backgroundLoader.sourceComponent = backgroundImage;
          }
          if (!privates.alpha) {
            backgroundLoader.sourceComponent = undefined;
          }
      }

      function showFader()
      {
          privates.originalParent = faderItem.parent;
          if (visualParent != null) {
              faderItem.parent = visualParent
          } else {
              var root = widgetSet.applicationBody;
              if (root != null) {
                  faderItem.parent = root;
              } else {
                 console.log("Error: Cannot find root");
              }
          }
          faderItem.fadedIn = true
      }
      function hideFader()
      {
          faderItem.fadedIn = false
          if (privates.originalParent)
              faderItem.parent = privates.originalParent;
      }
    }

    MouseArea {
        id: eventEater
        anchors.fill: parent
        enabled: privates.alpha != 0.0
        onClicked: parent.inactiveAreaClicked()
        hoverEnabled: true
        onPressedChanged: {
            if (!pressed)
                return
            if (!containsMouse) {
                parent.inactiveAreaClicked()
            }

        }
    }

    Component {
        id: backgroundImage
        BorderImage {
            source: backgroundSource
            width: faderItem.width
            height: faderItem.height
            opacity: privates.alpha
            border.left: 15
            border.top: 15
            border.right: 15
            border.bottom: 15
        }
    }
    Loader {
        id: backgroundLoader
    }

    states: [
        State {
            name: "visible"
            PropertyChanges {
                target: privates
                alpha: opacityWhenFadeIn
            }
            AnchorChanges {
                target: faderItem
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
            }
        }
    ]

    transitions: [
        Transition {
            from: ""; to: "visible"
            //reparent fader whenever it is going to be visible
            SequentialAnimation {
                ScriptAction {script: privates.showFader() }
                ParallelAnimation {
                    AnchorAnimation {alwaysRunToEnd: true; duration: fadeInDuration; easing.type: Easing.OutCubic}
                    PropertyAnimation {target: privates; properties: "alpha"; from: 0.0; to: opacityWhenFadeIn; duration: fadeInDuration; easing.type: Easing.OutCubic; }
                }
            }
        },
        Transition {
            from: "visible"; to: ""
            SequentialAnimation {
                PauseAnimation { duration: fadeOutDelay }
                ParallelAnimation {
                    AnchorAnimation {alwaysRunToEnd: true; duration: fadeOutDuration; easing.type: Easing.OutCubic}
                    PropertyAnimation {target: privates; properties: "alpha"; from: opacityWhenFadeIn; to: 0.0; duration: fadeOutDuration; easing.type: Easing.OutCubic; }
                }
                ScriptAction {script: privates.hideFader() }
            }
        }
    ]
}
