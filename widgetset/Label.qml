import QtQuick 1.1
import "." 1.0

Text {
    id: label
    property bool bold: false
    property bool italic: false

    height: paintedHeight
    StyledItem { id: labelTheme; styleName: "Label"}

    color: (labelTheme.style) ? labelTheme.style.fontColor : undefined
    font: (labelTheme.style) ? labelTheme.style.font : undefined
    verticalAlignment: Text.AlignVCenter
}
