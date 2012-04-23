// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/*
  Dock item to hold styled items; base for Toolbar, PageTab, etc
  */

StyledItem {

    property alias activeToolId: group.activeId
    property alias autoId: group.autoId
    default property alias content: body.data
    property alias contentFit: group.bodyFit
    property variant alignment: Qt.AlignJustify

    styleName: "DockStyle"
    id: dock
    width: 100
    height: 62

    Component {
        id: imageFrame
        BorderImage {
            anchors.fill: parent
            source: dock.style.imageUrl
            border.left: dock.style.imageBorders[0]
            border.top: dock.style.imageBorders[1]
            border.right: dock.style.imageBorders[2]
            border.bottom: dock.style.imageBorders[3]
        }
    }

    Component {
        id: gradientFrame
        Rectangle {
            anchors.fill: parent
            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 2
                color: dock.style.frameColor
                border.color: dock.style.frameColor
                border.width: dock.style.frameWidth
            }
            gradient: Gradient {
                GradientStop {
                    color: Qt.lighter(dock.style.fillColor, dock.style.gradientTones[0])
                    position: dock.style.gradientSteps[0]
                }
                GradientStop {
                    color: Qt.lighter(dock.style.fillColor, dock.style.gradientTones[1])
                    position: dock.style.gradientSteps[1]
                }
                GradientStop {
                    color: Qt.lighter(dock.style.fillColor, dock.style.gradientTones[2])
                    position: dock.style.gradientSteps[2]
                }
                GradientStop {
                    color: dock.style.fillColor
                    position: dock.style.gradientSteps[3]
                }
            }
        }
    }

    Loader {
        id: styleFrame
        z: -1
        anchors.fill: parent
        function setupFrame()
        {
            if (dock.style.imageUrl && (dock.style.imageUrl != undefined))
                sourceComponent = imageFrame
            else
                sourceComponent = gradientFrame
        }
    }

    CheckGroup {
        id: group
        anchors.fill: parent
        autoId: true
        bodyFit: CheckGroup.FitToGroup
        Row {
            id: body
            property int marginOffset: 0
            property real defSpacing: 5
            anchors {
                fill: parent
                leftMargin: 1 + marginOffset
                topMargin: 1
                rightMargin: 1 + marginOffset
                bottomMargin: 1
            }
            //width: parent.width
            spacing: layoutChildren()
            function layoutChildren()
            {
                if (group.bodyFit != CheckGroup.FitToGroup)
                    return 1
                var icount = body.children.length
                var fw = styleFrame.width - 2
                switch (dock.alignment) {
                case Qt.AlignHCenter:
                    var cw = children[0].width * icount + (icount - 1) * defSpacing
                    marginOffset = (fw - cw) / 2
                    return defSpacing
                case Qt.AlignJustify:
                    marginOffset = 0
                    return (fw - icount * children[0].width) / (icount - 1) - 1
                default:
                    return defSpacing
                }
            }
        }
    }
    Component.onCompleted: {
        styleFrame.setupFrame()
        //body.layoutChildren()
    }
    onStyleChanged: styleFrame.setupFrame()
    //onControlChanged: styleFrame.setupFrame()
    //onAlignmentChanged: body.layoutChildren()

}
