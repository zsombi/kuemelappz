#ifndef SCREEN_P_H
#define SCREEN_P_H

#endif // SCREEN_P_H
#include "screen.h"

class QDesktopWidget;
class ScreenPrivate
{
    Q_DECLARE_PUBLIC(Screen)
public:
    ScreenPrivate(Screen *qq);
    ~ScreenPrivate();
protected: // mobile platform API
    void initializeSensors();
    void toggleSensorActive(bool active);
public:

    Screen *q_ptr;
    QObject *sensor;
    QDesktopWidget *desktop;
    int appScreen;
    int screenCount;
    qreal screenRotation;
    qreal rotationCorrection;
    Screen::Orientation orientation;
    bool orientationLocked;

    void _q_updateScreenCount(int count);
    void _q_updateScreenSize(int screen);
    void _q_updateSensorData();
};

