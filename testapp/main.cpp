#include <QtGui/QApplication>
#include <QDir>
#include <QDebug>
//#include "qmlapplicationviewer.h"

#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <qplatformdefs.h> // MEEGO_EDITION_HARMATTAN

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef ENABLE_QML_TRACES
    QByteArray data = "1";
    qputenv("QML_IMPORT_TRACE", data);
#endif

    QDeclarativeView view;

    view.engine()->rootContext()->setContextProperty("appResAlias", "apptheme");
    view.engine()->rootContext()->setContextProperty("appResFile", "qml/testapp/apptheme.rcc");
    //view.setSource(QUrl::fromLocalFile("qml/testapp/main.qml"));
    view.setSource(QUrl("qrc:/main.qml"));

#if defined(Q_OS_SYMBIAN) || defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_SIMULATOR) || defined(Q_OS_SIMULATOR) || defined(Q_WS_MAEMO_5)
    view.showFullScreen();
#else
    view.show();
#endif

    return app.exec();
}
