import QtQuick 1.1
import "." 1.0

Text {
    id: label
    property bool bold: false
    property bool italic: false

    height: paintedHeight
    StyledItem { id: labelTheme; styleName: "Label"}

    font.family: (labelTheme.style) ? labelTheme.style.fontFamily : undefined
    font.pixelSize: labelTheme.textSize
    font.weight: labelTheme.style.fontWeight
    font.bold: label.bold
    font.italic: label.italic
    verticalAlignment: Text.AlignVCenter
}
