/**
  ApplicationItem - application's main window. Handles orientation change and holds layout properties.

  Types:
    Orientation - application's supported orientations
        Automatic - automatic orientation
        Portrait
        Landscape
        InverseLandscape

  Properties

    property orientation: Orientation
        Specifies the current orientation. The property has meaning for mobile platforms,
        on desktop its value is always Automatic.

    property orientationString: string
        String representation of orientation property.

    property lockToOrientation: Orientation
        Specifies to which orientation should teh application window be locked. The lock
        is reset when Automatic is set.


  */
#include "globaldefs.h"
#include "applicationitem.h"
#include "applicationitem_p.h"
#include "widgetset.h"
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeProperty>
#include "screen.h"

#ifdef MOBILE_SYSTEM
#include <QOrientationSensor>
#include <QOrientationReading>
QTM_USE_NAMESPACE
#endif

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

//#define TRACE_APPITEM

ApplicationItemPrivate::ApplicationItemPrivate(ApplicationItem *qq) :
    q_ptr(qq),
    screenSize(0,0),
    appView(0)
{
    WidgetSet::instance()->appWindow = q_ptr;
    foreach (QWidget *w, QApplication::allWidgets()) {
        appView = qobject_cast<QDeclarativeView*>(w);
        if (appView) {
            appView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
            appView->setAttribute(Qt::WA_LockPortraitOrientation);
            break;
        }
    }

    title = QApplication::applicationName();

    initializePlatform();
    QObject::connect(Screen::instance(), SIGNAL(orientationChanged()), q_ptr, SLOT(_q_sensorUpdate()));
}

void ApplicationItemPrivate::_q_sceneUpdate(const QSize &sceneSize)
{
    if (sceneSize != screenSize) {
#ifdef TRACE_APPITEM
        qDebug() << "Scene: " << sceneSize;
#endif
        resize(sceneSize);
    }
}

void ApplicationItemPrivate::_q_sensorUpdate()
{
    Screen::Orientation o = Screen::instance()->orientation();
    setOrientation(o);
}

void ApplicationItemPrivate::resize(const QSize &size)
{
    Q_Q(ApplicationItem);
    screenSize = size;
#ifdef TRACE_APPITEM
    qDebug() << "resize: " << screenSize;
#endif
    QDeclarativeProperty::write(q, "width", screenSize.width());
    QDeclarativeProperty::write(q, "height", screenSize.height());
}

void ApplicationItemPrivate::setOrientation(Screen::Orientation o)
{
    Q_Q(ApplicationItem);

    int shortEdge = qMin(screenSize.width(), screenSize.height());
    int longEdge = qMax(screenSize.width(), screenSize.height());
    QSize newSize(shortEdge, longEdge);
    if (o == Screen::Landscape || o == Screen::InverseLandscape) {
        newSize.transpose();
    }

#ifdef TRACE_APPITEM
    qDebug() << "setOrientation: " << screenSize << "::" << newSize << "@" << Screen::instance()->rotationAngle();
#endif

    QDeclarativeProperty::write(q, "rotation", Screen::instance()->rotationAngle());
    resize(newSize);

    emit q->orientationChanged();
}

//**************************************************************

ApplicationItem::ApplicationItem(QDeclarativeItem *parent) :
    StyledItem(parent),
    d_ptr(new ApplicationItemPrivate(this))
{
    //setFocusType(FocusGroup);
    //setFlag(ItemIsFocusable);
    //setFlag(ItemIsFocusScope);

    // set the default keys for non-mobile platforms (yet)
    if (!WidgetSet::instance()->mobilePlatform()) {
        installEventFilter(ControlAction::createAction(this, Qt::Key_Left, Qt::NoModifier, ControlAction::Controls, "focusOnPrevious"));
        installEventFilter(ControlAction::createAction(this, Qt::Key_Up, Qt::NoModifier, ControlAction::Controls, "focusOnPrevious"));
        installEventFilter(ControlAction::createAction(this, Qt::Key_Right, Qt::NoModifier, ControlAction::Controls, "focusOnNext"));
        installEventFilter(ControlAction::createAction(this, Qt::Key_Down, Qt::NoModifier, ControlAction::Controls, "focusOnNext"));
        installEventFilter(ControlAction::createAction(this, Qt::Key_Space, Qt::NoModifier, ControlAction::Controls, "invoked"));
        installEventFilter(ControlAction::createAction(this, Qt::Key_Return, Qt::NoModifier, ControlAction::Controls, "invoked"));
        installEventFilter(ControlAction::createAction(this, Qt::Key_Tab, Qt::NoModifier, ControlAction::Both, "focusOnNextGroup"));
        installEventFilter(ControlAction::createAction(this, Qt::Key_Backtab, Qt::ShiftModifier, ControlAction::Both, "focusOnPreviousGroup"));
    }
}

ApplicationItem::~ApplicationItem()
{

}

Screen::Orientation ApplicationItem::orientation() const
{
    return Screen::instance()->orientation();
}
QString ApplicationItem::title() const
{
    Q_D(const ApplicationItem);
    return d->title;
}
void ApplicationItem::setTitle(const QString &txt)
{
    Q_D(ApplicationItem);
    if (d->title != txt) {
        d->title = txt;
        emit titleChanged();
    }
}


void ApplicationItem::componentComplete()
{
    StyledItem::componentComplete();

#ifdef TRACE_APPITEM
    qDebug() << "AppItem completed";
#endif
    Q_D(ApplicationItem);
    d->platformCompleted();
    // set focus to the first group's element
    // topmost focus group is in the "body" property
    FocusControl *body = qobject_cast<FocusControl*>(WidgetSet::instance()->rootBodyItem());
    if (body)
        body->forceActiveFocus();
}

#include "moc_applicationitem.cpp"
