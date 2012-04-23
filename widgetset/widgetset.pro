TEMPLATE = lib
TARGET = widgetset
QT += declarative
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.zesoft.widgetset

CONFIG += mobility
MOBILITY += sensors

# Input
SOURCES += \
    widgetset_plugin.cpp \
    styleditem.cpp \
    stylemanager.cpp \
    applicationitem.cpp \
    checkgroup.cpp \
    widgetset.cpp \
    style.cpp \
    theme.cpp

HEADERS += \
    widgetset_plugin.h \
    styleditem.h \
    styleditem_p.h \
    stylemanager.h \
    applicationitem.h \
    applicationitem_p.h \
    globaldefs.h \
    checkgroup.h \
    checkgroup_p.h \
    widgetset.h \
    style.h \
    theme.h

OTHER_FILES = qmldir \
    theme_descriptor.xml \
    ApplicationWindow.qml \
    Page.qml \
    Dock.qml \
    Label.qml \
    ButtonControl.qml \
    Button.qml \
    ToolButton.qml \
    PageTab.qml \
    PageStack.js \
    PageStack.qml \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    StatusBar.qml \
    BusyIndicator.qml \
    FaderItem.qml \
    MenuPanel.qml \
    LineEdit.qml \
    CornerFramer.qml \
    ButtonStyle.qml \
    DefaultTheme.qml \
    IndicatorStyle.qml \
    StatusBarStyle.qml \
    CornerFramerStyle.qml \
    DockStyle.qml \
    ToolButtonStyle.qml \
    TextEditStyle.qml \
    LabelStyle.qml \
    ScrollBarStyle.qml \
    ScrollBar.qml \
    Scrollable.qml \
    InputPanel.qml \
    Utility.js

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

QML_FILES = ApplicationWindow.qml \
    Page.qml \
    Dock.qml \
    Label.qml \
    ButtonControl.qml \
    Button.qml \
    ToolButton.qml \
    PageTab.qml \
    PageStack.qml \
    PageStack.js \
    StatusBar.qml \
    BusyIndicator.qml \
    FaderItem.qml \
    MenuPanel.qml \
    LineEdit.qml \
    CornerFramer.qml \
    ButtonStyle.qml \
    DefaultTheme.qml \
    IndicatorStyle.qml \
    StatusBarStyle.qml \
    CornerFramerStyle.qml \
    DockStyle.qml \
    ToolButtonStyle.qml \
    TextEditStyle.qml \
    LabelStyle.qml \
    ScrollBarStyle.qml \
    ScrollBar.qml \
    Scrollable.qml \
    InputPanel.qml \
    Utility.js

qmldir.files = qmldir $$QML_FILES
symbian {
    TARGET.EPOCALLOWDLLDATA = 1
} else:unix {
    maemo5 | !isEmpty(MEEGO_VERSION_MAJOR) {
        installPath = /usr/lib/qt4/imports/$$replace(uri, \\., /)
    } else {
        installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
    }
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
} else {
    installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

RESOURCES += \
    default_theme.qrc

