/**
  WidgetSet - utility class providing various functionalities and type definitions
  towards the components defined in the toolkit. The insance is published towards
  QML through "widgetSet" variable and enums through WidgetSet type.

  Enums:

  FaderStyle - defines the FaderItem styles
    FadeInOut - fading will happen with all sides anchored to the visible parent
                giving the sensation of appearing from center of the parent
    FadeLeft - fading happens from the left side of the parent item
    FadeTop - fading happens from the top side of the parent item
    FadeRight - fading happens from the right side of the parent item
    FadeBottom - fading happens from the bottom side of the parent item

  PageStatus - defines page status types
    PageInactive - the page is in inactive state
    PageActivating - the page is prepared to be active
    PageActive - the page is active
    PageDeactivating - the page is on teh way to be deactivated

  Properties:
    property mobilePlatform: boolean
        Specifies whether the platform used is a mobile or desktop platform

    property busy: boolean
        Drives the system busy state. The system busy state is counter based, meaning that
        it has to be reset as many times that it got set in order to become false

    property applicationBody: Item
        Specifies the application's body item.

    property statusBar: Item
        Specifies the application's status bar item.

    property inputPanel: Item
        Specifies the application's input panel item.

*/
#include "widgetset.h"
#include <QDeclarativeItem>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeProperty>
#include <QApplication>
#include "applicationitem.h"
#include "globaldefs.h"

//#define TRACE_WIDGETSET

Q_GLOBAL_STATIC(WidgetSet, setInstance)

WidgetSet::WidgetSet(QObject *parent) :
    QObject(parent),
    appWindow(0),
    busyCount(0)
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

// initializes WidgetSet instance, publishes the object to the root context
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
        // app window item should have a property that defines the body item
        QVariant hdr = QDeclarativeProperty::read(appWindow, "body");
        QObject *ho = hdr.value<QObject*>();
        ret = qobject_cast<QDeclarativeItem*>(ho);
    }
    return ret;
}

QDeclarativeItem *WidgetSet::statusBar()
{
    ApplicationItem *appItem = qobject_cast<ApplicationItem*>(appWindow);
    if (appItem) {
        QVariant hdr = QDeclarativeProperty::read(appWindow, "statusBar");
        QObject *ho = hdr.value<QObject*>();
        return qobject_cast<QDeclarativeItem*>(ho);
    }
    return 0;
}

QDeclarativeItem *WidgetSet::inputPanel()
{
    ApplicationItem *appItem = qobject_cast<ApplicationItem*>(appWindow);
    if (appItem) {
        QVariant hdr = QDeclarativeProperty::read(appWindow, "inputPanel");
        QObject *ho = hdr.value<QObject*>();
        return qobject_cast<QDeclarativeItem*>(ho);
    }
    return 0;
}

bool WidgetSet::busy() const
{
    return (bool)(busyCount > 0);
}
void WidgetSet::setBusy(bool v)
{
    int prevCount = busyCount;
    if (v)
        busyCount++;
    else
        busyCount--;
    if (busyCount < 0)
        busyCount = 0;
    if (((prevCount <= 0) && (busyCount > 0)) ||
        ((prevCount > 0) && (busyCount <= 0)))
        emit busyChanged();
}



