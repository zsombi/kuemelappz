#include "widgetset_plugin.h"
#include "styleditem.h"
#include "stylemanager.h"
#include "applicationitem.h"
#include "checkgroup.h"
#include "widgetset.h"
#include "style.h"
#include "theme.h"

#include <QtDeclarative/qdeclarative.h>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QApplication>

#include <QFont>

void WidgetsetPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    QDeclarativeExtensionPlugin::initializeEngine(engine, uri);

    QCoreApplication *app = QApplication::instance();
    connect(engine, SIGNAL(quit()), app, SLOT(quit()));

    StyleManager::initialize(engine->rootContext());
    WidgetSet::initialize(engine->rootContext());
}

void WidgetsetPlugin::registerTypes(const char *uri)
{
    // @uri com.zesoft.widgetset
    qmlRegisterType<Style>(uri, 1, 0, "Style");
    qmlRegisterType<Theme>(uri, 1, 0, "Theme");

    qmlRegisterType<StyledItem>(uri, 1, 0, "StyledItem");
    qmlRegisterType<CheckGroup>(uri, 1, 0, "CheckGroup");
    qmlRegisterType<ApplicationItem>(uri, 1, 0, "ApplicationItem");
    qmlRegisterUncreatableType<WidgetSet>(uri, 1, 0, "WidgetSet", "Component type");
}

Q_EXPORT_PLUGIN2(Widgetset, WidgetsetPlugin)

