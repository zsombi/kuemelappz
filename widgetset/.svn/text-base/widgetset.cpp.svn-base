#include "widgetset.h"
#include <QDeclarativeItem>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QApplication>
#include "applicationitem.h"
#include "globaldefs.h"

Q_GLOBAL_STATIC(WidgetSet, setInstance)

QDeclarativeItem *locateAppItem(QDeclarativeItem *w)
{
    if (!w || (w && w->children().isEmpty()))
        return 0;
    foreach(QObject *pl, w->children()) {
        ApplicationItem *appItem = qobject_cast<ApplicationItem*>(pl);
        if (appItem) {
            return appItem->findChild<QDeclarativeItem*>("AppBodyItem");
        } else if (!pl->children().isEmpty()) {
            QDeclarativeItem *ret = locateAppItem(qobject_cast<QDeclarativeItem*>(pl));
            if (ret)
                return ret;
        }
    }

    return 0;
}


WidgetSet::WidgetSet(QObject *parent) :
    QObject(parent),
    appWindow(0)
{
}

WidgetSet::~WidgetSet()
{
}

bool WidgetSet::mobilePlatform()
{
#ifdef MOBILE_SYSTEM
    return true;
#else
    return true;
#endif

}

void WidgetSet::initialize(QDeclarativeContext *context)
{
    WidgetSet *obj = setInstance();

    context->setContextProperty("widgetSet", obj);
}
WidgetSet *WidgetSet::instance()
{
    return setInstance();
}

QDeclarativeItem *WidgetSet::rootBodyItem()
{
    QDeclarativeItem *ret = 0;
    if (appWindow) {
        ret = appWindow->findChild<QDeclarativeItem*>("AppBodyItem");;
    } else {
        foreach (QWidget *w, QApplication::allWidgets()) {
            QDeclarativeView *appView = qobject_cast<QDeclarativeView*>(w);
            if (appView) {
                QGraphicsItem *ro = appView->rootObject();
                ret = locateAppItem(qobject_cast<QDeclarativeItem*>(ro));
                break;
            }
        }
    }
    return ret;
}

QDeclarativeItem *WidgetSet::inputPanel()
{
    ApplicationItem *appItem = qobject_cast<ApplicationItem*>(appWindow);
    return (appItem) ? appItem->inputPanel() : 0;
}

