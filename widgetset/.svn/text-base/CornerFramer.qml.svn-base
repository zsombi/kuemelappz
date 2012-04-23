// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/*
  Component giving the rounded corner frame sensation for items... as
  radius doesn't clip the Rectangle content...
  Style: CornerFramerStyle

  todo: maye a better name should be given... ehh
  */
StyledItem {
    id: cornerFramer

    width: 100
    height: 100
    Image {
        anchors {left: parent.left; top: parent.top}
        fillMode: Image.PreserveAspectFit
        source: cornerFramer.style.sourceLeftTop
    }
    Image {
        anchors {right: parent.right; top: parent.top}
        fillMode: Image.PreserveAspectFit
        source: cornerFramer.style.sourceRightTop
    }
    Image {
        anchors {right: parent.right; bottom: parent.bottom}
        fillMode: Image.PreserveAspectFit
        source: cornerFramer.style.sourceRightBottom
    }
    Image {
        anchors {left: parent.left; bottom: parent.bottom}
        fillMode: Image.PreserveAspectFit
        source: cornerFramer.style.sourceLeftBottom
    }
}
