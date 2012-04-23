#ifndef WIDGETSET_PLUGIN_H
#define WIDGETSET_PLUGIN_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

class WidgetsetPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
    
public:
    void initializeEngine(QDeclarativeEngine *engine, const char *uri);
    void registerTypes(const char *uri);
};

#endif // WIDGETSET_PLUGIN_H

