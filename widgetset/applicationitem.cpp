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

#define TRACE_APPITEM

ApplicationItemPrivate::ApplicationItemPrivate(ApplicationItem *qq) :
    q_ptr(qq),
    orientation(ApplicationItem::Landscape),
    orientationLocked(false),
    sensor(0),
    screenSize(0,0),
    appView(0)
{
    WidgetSet::instance()->appWindow = q_ptr;
#ifdef Q_OS_SYMBIAN
    //QCoreApplication::setAttribute(Qt::AA_S60DontConstructApplicationPanes);
#endif
    foreach (QWidget *w, QApplication::allWidgets()) {
        appView = qobject_cast<QDeclarativeView*>(w);
        if (appView) {
#ifdef Q_OS_SYMBIAN
            CAknAppUi* appUi = dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
            TRAPD(error,
                if (appUi)
                    appUi->SetOrientationL(CAknAppUi::EAppUiOrientationPortrait);
            );
            Q_UNUSED(error)
#endif
            // connect scene update to catch screen size
            q_ptr->connect(appView, SIGNAL(sceneResized(QSize)), SLOT(_q_sceneUpdate(const QSize&)), Qt::QueuedConnection);
            q_ptr->connect(q_ptr, SIGNAL(rotationChanged()), SLOT(_q_rotation()));
            appView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
            appView->setAttribute(Qt::WA_LockPortraitOrientation);
            break;
        }
    }
#if defined(MOBILE_SYSTEM)// && !defined(MEEGO_EDITION_HARMATTAN)

#ifdef MEEGO_EDITION_HARMATTAN
    // Harmattan launches the application in Landscape mode.
    orientation = ApplicationItem::Landscape;
#endif
    // use orientation sensors to rotate layout
    sensor = new QOrientationSensor(q_ptr);
    q_ptr->connect(sensor, SIGNAL(readingChanged()), SLOT(_q_sensorUpdate()));
    qobject_cast<QOrientationSensor*>(sensor)->setActive(true);
#else
    orientation = ApplicationItem::Portrait;
#endif
}

ApplicationItem::ApplicationItem(QDeclarativeItem *parent) :
    StyledItem(parent),
    d_ptr(new ApplicationItemPrivate(this))
{
}

ApplicationItem::~ApplicationItem()
{

}

ApplicationItem::Orientation ApplicationItem::orientation() const
{
    Q_D(const ApplicationItem);
    return d->orientation;
}
QString ApplicationItem::orientationString() const
{
    Q_D(const ApplicationItem);
    if (d->orientation == ApplicationItem::Automatic)
        return QString();
    int index = metaObject()->indexOfEnumerator("Orientation");
    Q_ASSERT(index != -1);
    QMetaEnum enumerator = metaObject()->enumerator(index);
    return QLatin1String(enumerator.valueToKey(d->orientation));
}
ApplicationItem::Orientation ApplicationItem::lockOnOrientation() const
{
    Q_D(const ApplicationItem);
    return d->orientation;
}
void ApplicationItem::setLockOnOrientation(Orientation o)
{
#ifdef MOBILE_SYSTEM
    Q_D(ApplicationItem);
    bool lock = (o == ApplicationItem::Automatic);

    if (lock != d->orientationLocked) {
        d->orientationLocked = lock;
        if (lock)
            d->orientation = o;
        qobject_cast<QOrientationSensor*>(d->sensor)->setActive(!lock);
        d->setOrientation(o);
    }
#endif
}

//**************************************************************

void ApplicationItemPrivate::_q_rotation()
{
#ifdef TRACE_APPITEM
    Q_Q(ApplicationItem);
    qDebug() << "rotaton: " << QDeclarativeProperty::read(q, "rotation").toInt();
#endif
}

void ApplicationItemPrivate::_q_sceneUpdate(const QSize &sceneSize)
{
    Q_Q(ApplicationItem);
    if (sceneSize != screenSize) {
        // set property to invoke property binding
#ifdef TRACE_APPITEM
        qDebug() << "Scene: " << sceneSize;
#endif

#ifdef MEEGO_EDITION_HARMATTAN
        if (!sceneSize.isEmpty()) {
            QSize s(sceneSize);
            setOrientation(ApplicationItem::Portrait);
            s.transpose();
            resize(s);
        }
#else
        resize(sceneSize);
#endif
    }
}

void ApplicationItemPrivate::_q_sensorUpdate()
{
    if (orientationLocked)
        return;
#ifdef MOBILE_SYSTEM
    QOrientationReading *reading = qobject_cast<QOrientationSensor*>(sensor)->reading();
    ApplicationItem::Orientation o = orientation;
    switch (reading->orientation()) {
    case QOrientationReading::LeftUp:
        o = ApplicationItem::InverseLandscape;
        break;
    case QOrientationReading::RightUp:
        o = ApplicationItem::Landscape;
        break;
    default:
        o = ApplicationItem::Portrait;
        break;
    }
    setOrientation(o);
#endif
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

void ApplicationItemPrivate::setOrientation(ApplicationItem::Orientation o)
{
    Q_Q(ApplicationItem);

    if (this->orientation != o) {
        orientation = o;
        // adjust screen size

        int shortEdge = qMin(screenSize.width(), screenSize.height());
        int longEdge = qMax(screenSize.width(), screenSize.height());
        QSize newSize(shortEdge, longEdge);
        if (orientation == ApplicationItem::Landscape || orientation == ApplicationItem::InverseLandscape) {
            newSize.transpose();
        }

        int oldRotation = QDeclarativeProperty::read(q, "rotation").toInt();
        int newRotation = oldRotation;
        // set item rotation
        switch (o) {
        case ApplicationItem::InverseLandscape:
            newRotation = 270;
            break;
        case ApplicationItem::Landscape:
            newRotation = 90;
            break;
        default:
            newRotation = 0;
            break;
        }
#ifdef MEEGO_EDITION_HARMATTAN
        newRotation -= 90;
#endif

#ifdef TRACE_APPITEM
        qDebug() << "setOrientation: " << screenSize << "::" << newSize << "@" << newRotation << "(" << oldRotation << ")";
#endif

        if (newRotation != oldRotation)
            QDeclarativeProperty::write(q, "rotation", newRotation);

        resize(newSize);

        emit q->orientationChanged();
    }
}

void ApplicationItem::componentComplete()
{
    StyledItem::componentComplete();
    return;

#ifdef TRACE_APPITEM
    qDebug() << "AppItem completed";
#endif

#ifdef MEEGO_EDITION_HARMATTAN
    // the platform launches apps in Landscape mode, expected is to be in Portrait.
    // Therefore the layout is moved to portrait

    Q_D(ApplicationItem);
    d->setOrientation(ApplicationItem::Portrait);
#endif
}

#include "moc_applicationitem.cpp"
