// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

// todo: Old component, should it be styled?

Rectangle {
    id: faderItem
    /*** @public interface
      */
    property double fadeOpacity: 1.0
    property url backgroundSource: ""
    property Item visualParent: null
    property int fadeOutDelay: 175
    property int fadeInDuration: 175
    property int fadeOutDuration: 175
    property int fadeStyle: WidgetSet.FadeInOut
    property bool isOpen: false
    property bool closeOnInactiveAreaClick: true
    property int faderMarginLeft: -1
    property int faderMarginTop: -1
    property int faderMarginRight: -1
    property int faderMarginBottom: -1

    signal outerFrameClicked()
    signal opened()
    signal closed()

    function open(withFadeStyle)
    {
        if (withFadeStyle == undefined)
            fadeStyle = WidgetSet.FadeInOut
        else
            fadeStyle = withFadeStyle
        state = 'visible'
    }
    function close()
    {
        state = ''
    }

    /*** @internals
      */
    onOuterFrameClicked: if (closeOnInactiveAreaClick) close()
    color: backgroundSource != "" ? "transparent" : Qt.rgba(0.05, 0.05, 0.05, privates.alpha)
    z: Number.MAX_VALUE
    anchors {
        margins: -1 // enlarge it to avoid artefacts appearing upon orientation change
        leftMargin: {
            if (fadeStyle == WidgetSet.FadeFromLeft)
                return -1
            return faderMarginLeft
        }
        left: {
            if (fadeStyle == WidgetSet.FadeFromLeft)
                return undefined
            else if (fadeStyle == WidgetSet.FadeFromRight)
                return parent.right
            return parent.left
        }

        topMargin: {
            if (fadeStyle == WidgetSet.FadeFromTop)
                return -1
            return faderMarginTop
        }
        top: {
            if (fadeStyle == WidgetSet.FadeFromTop)
                return undefined
            else if (fadeStyle == WidgetSet.FadeFromBottom)
                return parent.bottom
            return parent.top
        }

        rightMargin: {
            if (fadeStyle == WidgetSet.FadeFromRight)
                return -1
            return faderMarginRight
        }
        right: {
            if (fadeStyle == WidgetSet.FadeFromRight)
                return undefined
            else if (fadeStyle == WidgetSet.FadeFromLeft)
                return parent.left
            return parent.right
        }

        bottomMargin: {
            if (fadeStyle == WidgetSet.FadeFromBottom)
                return -1
            return faderMarginBottom
        }
        bottom: {
            if (fadeStyle == WidgetSet.FadeFromBottom)
                return undefined
            else if (fadeStyle == WidgetSet.FadeFromTop)
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
              var root = widgetSet.rootBodyItem();
              if (root != null) {
                  faderItem.parent = root;
              } else {
                 console.log("Error: Cannot find root");
              }
          }
          faderItem.isOpen = true
          faderItem.opened()
      }
      function hideFader()
      {
          faderItem.isOpen = false
          faderItem.closed()
          if (privates.originalParent)
              faderItem.parent = privates.originalParent;
      }
    }

    MouseArea {
        id: eventEater
        anchors.fill: parent
        enabled: privates.alpha != 0.0
        onClicked: parent.outerFrameClicked()
        hoverEnabled: true
        onPressedChanged: {
            if (!pressed)
                return
            console.debug("pressed")
            if (!containsMouse) {
                console.debug("outside click")
                parent.outerFrameClicked()
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
                alpha: fadeOpacity
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
                    PropertyAnimation {target: privates; properties: "alpha"; from: 0.0; to: fadeOpacity; duration: fadeInDuration; easing.type: Easing.OutCubic; }
                }
            }
        },
        Transition {
            from: "visible"; to: ""
            SequentialAnimation {
                PauseAnimation { duration: fadeOutDelay }
                ParallelAnimation {
                    AnchorAnimation {alwaysRunToEnd: true; duration: fadeOutDuration; easing.type: Easing.OutCubic}
                    PropertyAnimation {target: privates; properties: "alpha"; from: fadeOpacity; to: 0.0; duration: fadeOutDuration; easing.type: Easing.OutCubic; }
                }
                ScriptAction {script: privates.hideFader() }
            }
        }
    ]
}
