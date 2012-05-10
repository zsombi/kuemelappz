#include "screen.h"
#include "screen_p.h"
#include "globaldefs.h"

#include <QDesktopWidget>
#include <QDesktopServices>
#include <QApplication>

#ifdef MOBILE_SYSTEM
#include <QOrientationSensor>
#include <QOrientationReading>
QTM_USE_NAMESPACE
#endif

#if !defined(MOBILE_SYSTEM)
void ScreenPrivate::initializeSensors()
{
    orientationLocked = true;
}

void ScreenPrivate::toggleSensorActive(bool active)
{
    Q_UNUSED(active)
}

void ScreenPrivate::_q_updateSensorData()
{

}

#else

void ScreenPrivate::initializeSensors()
{
    Q_Q(Screen);
    // use orientation sensors to rotate layout
    sensor = new QOrientationSensor(q);
    QObject::connect(sensor, SIGNAL(readingChanged()), q, SLOT(_q_updateSensorData()));
    qobject_cast<QOrientationSensor*>(sensor)->setActive(true);

#ifdef MEEGO_EDITION_HARMATTAN
    rotationCorrection = -90;
    orientation = Screen::Landscape;
#endif
}

void ScreenPrivate::toggleSensorActive(bool active)
{
    qobject_cast<QOrientationSensor*>(sensor)->setActive(active);
}

void ScreenPrivate::_q_updateSensorData()
{
    if (orientationLocked)
        return;

    QOrientationReading *reading = qobject_cast<QOrientationSensor*>(sensor)->reading();
    Screen::Orientation o = orientation;
    switch (reading->orientation()) {
    case QOrientationReading::LeftUp:
        o = Screen::InverseLandscape;
        screenRotation = 270;
        break;
    case QOrientationReading::RightUp:
        o = Screen::Landscape;
        screenRotation = 90;
        break;
    default:
        o = Screen::Portrait;
        screenRotation = 0;
        break;
    }
    screenRotation += rotationCorrection;

    if (orientation != o) {
        orientation = o;
        Q_Q(Screen);
        emit q->orientationChanged();
    }
}

#endif
