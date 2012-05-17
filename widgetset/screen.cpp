/**
  Screen - provides information about the current screen, serves for resolution and
  DPI calculation.

  */
#include "screen.h"
#include "screen_p.h"
#include "globaldefs.h"
#include <qmath.h>

#include <QDesktopWidget>
#include <QDesktopServices>
#include <QApplication>

//#define TRACE_SCREEN

#define CALCULATE_DPI

#define DISPLAY_TYPE_LIMIT_SMALL        (qreal)3.2
#define DISPLAY_TYPE_LIMIT_NORMAL       (qreal)4.5
#define DISPLAY_TYPE_LIMIT_LARGE        (qreal)7.0
#define DISPLAY_DENSITY_LIMIT_SMALL     (qreal)140.0
#define DISPLAY_DENSITY_LIMIT_MEDIUM    (qreal)180.0
#define DISPLAY_DENSITY_LIMIT_LARGE     (qreal)270.0

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

    // emit displayChanged
    QMetaObject::invokeMethod(q_ptr, "displayChanged", Qt::QueuedConnection);
}
ScreenPrivate::~ScreenPrivate()
{
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
    qreal sdpi = 0.0;
#ifdef CALCULATE_DPI
    int w = width();
    int h = height();
    qreal physicalDisplaySize = 0.0;
    if (w*h == 640*360)
        physicalDisplaySize = 3.5;
    else if (w*h == 640*480)
        physicalDisplaySize = 2.46;
    else if (w*h == 320*240)
        physicalDisplaySize = 2.8;
    else if (w*h == 800*480)
        physicalDisplaySize = 3.5;
    else
        physicalDisplaySize = 3.5;

    qreal width_r = w;
    qreal height_r = h;
    sdpi = sqrt(width_r*width_r + height_r*height_r) / physicalDisplaySize;
#else
    sdpi = QDesktopWidget().physicalDpiX();
#endif
#ifdef TRACE_SCREEN
    qDebug() << "dpi=" <<sdpi <<", logicalX="<<QDesktopWidget().logicalDpiX()
                <<", logicalY="<<QDesktopWidget().logicalDpiY()
                  <<", physicalX="<<QDesktopWidget().physicalDpiX()
                    <<", physicalY="<<QDesktopWidget().physicalDpiY();
#endif
    return sdpi;
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
        // emit three signals: start, action and end
        emit orientationChangeStarted();
        emit orientationChanged();
        emit orientationChangeEnded();
    }
}

qreal Screen::rotationAngle() const
{
    Q_D(const Screen);
    return d->screenRotation;
}

Screen::DisplayType Screen::displayType() const
{
    int w = width();
    int h = height();
    const qreal diagonal = qSqrt(static_cast<qreal>(w*w + h*h)) / dpi();

    if (diagonal < DISPLAY_TYPE_LIMIT_SMALL)
        return Small;
    else if (diagonal < DISPLAY_TYPE_LIMIT_NORMAL)
        return Normal;
    else if (diagonal < DISPLAY_TYPE_LIMIT_LARGE)
        return Large;
    // neither of these, so return desktop (tablets belong here) type
    return Desktop;
}

QString Screen::typeString() const
{
    DisplayType dt = displayType();
    int index = metaObject()->indexOfEnumerator("DisplayType");
    Q_ASSERT(index != -1);
    QMetaEnum enumerator = metaObject()->enumerator(index);
    return QLatin1String(enumerator.valueToKey(dt));
}

Screen::DisplayDensity Screen::displayDensity() const
{
    const qreal sdpi = dpi();
    if (sdpi < DISPLAY_DENSITY_LIMIT_SMALL)
        return Low;
    else if (sdpi < DISPLAY_DENSITY_LIMIT_MEDIUM)
        return Medium;
    else if (sdpi < DISPLAY_DENSITY_LIMIT_LARGE)
        return High;

    return ExtraHigh;
}

QString Screen::densityString() const
{
    DisplayDensity dd = displayDensity();
    int index = metaObject()->indexOfEnumerator("DisplayDensity");
    Q_ASSERT(index != -1);
    QMetaEnum enumerator = metaObject()->enumerator(index);
    return QLatin1String(enumerator.valueToKey(dd));
}

#include "moc_screen.cpp"
