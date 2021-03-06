// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0
import "Utility.js" as Utils

// Single line editor, themable with TextEditStyle
StyledItem {
    /** public API
      */
    // input panel flags (see WidgetSet.InputLayoutFlags)
    property int inputFlags: WidgetSet.InputWithClose
    // hint text
    property alias hint: hintText.text
    // editor data
    property alias text: editor.text
    // font object for the hint
    property alias hintFont: hintText.font
    // font object for the editor
    property alias editorFont: editor.font
    // specifies whether the input should be secret or not
    property bool password: false
    // editor's mask
    property alias inputMask: editor.inputMask
    // editor's height - deprecate?
    property alias editorHeight: frame.height

    /** Internals
      */
    id: textEdit
    width: 100
    height: THEME.sizes.defLineEditHeight
    styleName: "ThemeLineEdit"
    objectName: "TextEditor"
    focusType: StyledItem.Focusable
    attachedControl: editor
    focus: true
    //properties.fillColor: "#5e2e2e"

    BorderImage {
        id: frame
        source: textEdit.style.image
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: parent.height
        //z: 50
        border.left: textEdit.style.imageBorders[0]; border.top: textEdit.style.imageBorders[1]
        border.right: textEdit.style.imageBorders[2]; border.bottom: textEdit.style.imageBorders[3]

        Text {
            id: hintText
            color: textEdit.style.hintColor
            text: "<Entry>"
            smooth: true
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            font: textEdit.style.hintFont
            visible: (editor.text == "") && (editor.focus == false)
        }

        TextInput {
            id: editor
            focus: true
            anchors.fill: parent
            anchors.leftMargin: textEdit.style.imageBorders[0]; anchors.topMargin: textEdit.style.imageBorders[1]
            anchors.rightMargin: textEdit.style.imageBorders[2]; anchors.bottomMargin: textEdit.style.imageBorders[3]
            width: parent.width
            clip: true
            font: textEdit.style.editorFont
            echoMode: (password) ? TextInput.Password : TextInput.Normal

            activeFocusOnPress: Utils.usePlatformInput()
            MouseArea {
                anchors.fill: parent
                enabled: !editor.activeFocusOnPress
                onClicked: {
                    if (!editor.activeFocus) {
                        editor.forceActiveFocus()
                        Utils.openInputPanel(editor, inputFlags, "QWERTY")
                    }
                }
                onPressAndHold: {
                    Utils.closeInputPanel(editor)
                }
            }
        }
    }
}
