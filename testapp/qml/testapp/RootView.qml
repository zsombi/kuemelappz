import QtQuick 1.0

Rectangle {
    id: rectangle2
    property color fill: "#000000"
    width: 360
    height: 80
    radius: 0
    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#555454"
        }

        GradientStop {
            position: 0.28
            color: "#292929"
        }

        GradientStop {
            position: 0.5
            color: "#000000"
        }

        GradientStop {
            position: 1
            color: "#000000"
        }
    }

    Rectangle {
        id: rectangle1
        y: 10
        width: 98
        height: 52
        color: "#ffffff"
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: text1
        x: 178
        y: 33
        width: 80
        height: 20
        color: "#b9a4a4"
        text: "text"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        smooth: true
        font.pixelSize: 12
    }

}
