// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

StyledItem {
    id: background
    width: 100
    height: 62

    Component {
        id: imageBackground
        Image {
            anchors.fill: parent
            source: background.style.image
            fillMode: background.style.fillMode
        }
    }

    Component {
        id: fillBackground
        Rectangle {
            anchors.fill: parent
            color: background.style.color
            radius: background.style.radius
        }
    }

    Component {
        id: gradientBackground
        Rectangle {
            anchors.fill: parent
            color: "transparent"
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
            if (background.style.image && (background.style.image !== undefined) && (background.style.image != "")) {
                console.debug("image background::" + background.style.image)
                sourceComponent = imageBackground
            } else if (background.style.gradientStops) {
                console.debug("gradientBackground")
                sourceComponent = gradientBackground
            } else {
                console.debug("fillBackground")
                sourceComponent = fillBackground
            }
        }
    }

    onStyleChanged: styleLoader.setupStyle()

}
