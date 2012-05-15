// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "." 1.0

/*
  Dock item to hold styled items; base for Toolbar, PageTab, etc
  */

Background {
    id: dock
    // current layout object
    property Item dockLayout: null

    // function to set the layout
    function setLayout(layout)
    {
        if (dockLayout && (layout != dockLayout)) {
            // parent it back to the original
            dockLayout.parent = privates.prevLayoutParent
            privates.prevLayoutParent = null
        }
        dockLayout = layout
        if (dockLayout) {
            privates.prevLayoutParent = dockLayout.parent
            dockLayout.parent = dock
            dockLayout.toolBar = dock
        }
    }

    QtObject {
        id: privates
        property Item prevLayoutParent
    }

    styleName: "ThemeToolbarFrame" //"dockFrame"
    width: parent ? parent.width : 0
    height: THEME.sizes.dockHeight
}
