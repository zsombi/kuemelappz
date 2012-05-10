/**
  Screen - provides information about the current screen, serves for resolution and
  DPI calculation.

  */
#include "screen.h"
#include "screen_p.h"
#include "globaldefs.h"

#include <QDesktopWidget>
#include <QDesktopServices>
#include <QApplication>

#define TRACE_SCREEN

Q_GLOBAL_STATIC(Screen, screenInst)

ScreenPrivate::ScreenPrivate(Screen *qq) :
    q_ptr(qq),
    desktop(QApplication::desktop()),
    screenRotation(0.0),
    rotationCorrection(0.0),
    orientation(Screen::Portrait),
    orientationLocked(false)
{
    appScreen = desktop->primaryScreen();
    screenCount = desktop->screenCount();

    QObject::connect(desktop, SIGNAL(screenCountChanged(int)), qq, SLOT(_q_updateScreenCount(int)));

    initializeSensors();
}
ScreenPrivate::~ScreenPrivate()
{
    if (sensor)
        QObject::disconnect(sensor, SIGNAL(readingChanged()), q_ptr, SLOT(_q_updateSensorData()));
    QObject::disconnect(desktop, SIGNAL(screenCountChanged(int)), q_ptr, SLOT(_q_updateScreenCount(int)));
}

void ScreenPrivate::_q_updateScreenCount(int count)
{
    screenCount = count;
}
void ScreenPrivate::_q_updateScreenSize(int screen)
{
    Q_Q(Screen);
    emit q->displayChanged();
}

/*************************************************************************
 *************************************************************************/
Screen::Screen(QObject *parent) :
    QObject(parent),
    d_ptr(new ScreenPrivate(this))
{
}
Screen::~Screen()
{}

Screen *Screen::instance()
{
    return screenInst();
}

int Screen::width() const
{
    Q_D(const Screen);
#ifdef TRACE_SCREEN
    qDebug() << "Screen width=" << d->desktop->screenGeometry(d->appScreen).width();
#endif
    return d->desktop->screenGeometry(d->appScreen).width();
}
int Screen::height() const
{
    Q_D(const Screen);
#ifdef TRACE_SCREEN
    qDebug() << "Screen height=" << d->desktop->screenGeometry(d->appScreen).height();
#endif
    return d->desktop->screenGeometry(d->appScreen).height();
}
qreal Screen::dpi() const
{
    Q_D(const Screen);
    return d->desktop->physicalDpiX();
}
Screen::Orientation Screen::orientation() const
{
    Q_D(const Screen);
    return d->orientation;
}
QString Screen::orientationString() const
{
    Q_D(const Screen);
    if (d->orientation == Screen::Automatic)
        return QString();
    int index = metaObject()->indexOfEnumerator("Orientation");
    Q_ASSERT(index != -1);
    QMetaEnum enumerator = metaObject()->enumerator(index);
    return QLatin1String(enumerator.valueToKey(d->orientation));
}
Screen::Orientation Screen::lockOnOrientation() const
{
    Q_D(const Screen);
    return d->orientation;
}
void Screen::setLockOnOrientation(Orientation o)
{
    Q_D(Screen);
    bool lock = (o == Screen::Automatic);

    if (lock != d->orientationLocked) {
        d->orientationLocked = lock;
        if (lock)
            d->orientation = o;
        d->toggleSensorActive(!lock);
        emit orientationChanged();
        //d->setOrientation(o);
    }
}

qreal Screen::rotationAngle() const
{
    Q_D(const Screen);
    return d->screenRotation;
}

#include "moc_screen.cpp"
