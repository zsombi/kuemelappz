TARGET = app
QT += declarative
# Add more folders to ship with the application, here
folder_01.source = qml/testapp
folder_01.path = qml/testapp

#DEPLOYMENTFOLDERS += folder_01
#CONFIG += qtquickdeploy
#CONFIG -= resources
#CONFIG += deployqml #binresource

PKG_DEPLOY = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH += com/zesoft/widgetset

symbian: {
    TARGET.UID3 = 0xEDDAEDDA
    load(armcc_warnings)
}
#symbian: TARGET.UID3 = 0x101F84EB # File Manager UID, to try out eclipsing

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices
#symbian:TARGET.CAPABILITY += WriteDeviceData ReadDeviceData

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER
#DEFINES += ENABLE_QML_TRACES

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
!symbian: CONFIG += mobility #needed to add mobility path to environment
#MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

# Please do not modify the following two lines. Required for deployment.
#include(qmlapplicationviewer/qmlapplicationviewer.pri)
#qtcAddDeployment()

QML_FILES = \
    qml/testapp/View3.qml \
    qml/testapp/View2.qml \
    qml/testapp/View1.qml \
    qml/testapp/main.qml \
    qml/testapp/View4.qml

TARGETPATH = qml/testapp/

#RESOURCE_FILES += apptheme.qrc
#RESOURCE_TARGET = $$TARGETPATH
CONFIG += binresource
RESOURCES += resources.qrc

OTHER_FILES += \
    $$QML_FILES \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/testapp/bin
    INSTALLS += target
}

contains(MEEGO_EDITION,harmattan) {
    icon.files = testapp.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    INSTALLS += icon
}

contains(MEEGO_EDITION,harmattan) {
    desktopfile.files = testapp.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile
}
