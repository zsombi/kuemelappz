/**
  Background - background handling styled item. Can handle image, solid fill
  or gradient fill backgrounds. The gradient fills are limited to 4 stops and
  3 tones of the fill color. The first 3 stops are lighter tones of the fill
  color.

  The style element is declared by BackgroundStyle. The type of the background
  is decided based on the following principle:
  1. if the image is defined, the background is filled either using BorderImage
     (if the imageBorders property is specified) or using Image element with the
     mode specified in fillMode
  2. otherwise, if the gradientStops si defined, the background is filled with
     the color and with the tones specified in gradientTones
  3. othrwise, the background is filled with the color in solid

  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    id: background
    width: 100
    height: 62
    objectName: "SI_bkgnd+"+parent.objectName
    focusType: StyledItem.Decorative

    Component {
        id: imageBackground
        Image {
            id: frame
            anchors.fill: parent
            source: background.style.image
            fillMode: (background.style.fillMode != undefined) ? background.style.fillMode : Image.Stretch
        }
    }

    Component {
        id: borderImageBackground
        BorderImage {
            id: frame
            anchors.fill: parent
            source: background.style.image

            border.left: (background.style.imageBorders == undefined) ? 0 : background.style.imageBorders[0]
            border.top:  (background.style.imageBorders == undefined) ? 0 : background.style.imageBorders[1]
            border.right: (background.style.imageBorders == undefined) ? 0 : background.style.imageBorders[2]
            border.bottom: (background.style.imageBorders == undefined) ? 0 : background.style.imageBorders[3]

        }
    }

    Component {
        id: solidFillBackground
        Rectangle {
            anchors.fill: parent
            color: background.style.color
            radius: (background.style.radius != undefined) ? background.style.radius : 0
        }
    }

    Component {
        id: gradientFillBackground
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: (background.style.radius != undefined) ? background.style.radius : 0
            gradient: Gradient {
                GradientStop {
                    color: Qt.lighter(background.style.color, background.style.gradientTones[0])
                    position: background.style.gradientStops[0]
                }
                GradientStop {
                    color: Qt.lighter(background.style.color, background.style.gradientTones[1])
                    position: background.style.gradientStops[1]
                }
                GradientStop {
                    color: Qt.lighter(background.style.color, background.style.gradientTones[2])
                    position: background.style.gradientStops[2]
                }
                GradientStop {
                    color: background.style.color
                    position: background.style.gradientStops[3]
                }
            }
        }
    }

    Loader {
        id: styleLoader
        anchors.fill: parent
        function setupStyle()
        {
            if (!background.style)
                return
            //console.debug("background.style= "+background.style)
            if (background.style.image && (background.style.image !== undefined) && (background.style.image != "")) {
                if (background.style.imageBorders != undefined)
                    sourceComponent = borderImageBackground
                else
                    sourceComponent = imageBackground
            } else if (background.style.gradientStops) {
                sourceComponent = gradientFillBackground
            } else {
                sourceComponent = solidFillBackground
            }
        }
    }

    onStyleChanged: styleLoader.setupStyle()

}
