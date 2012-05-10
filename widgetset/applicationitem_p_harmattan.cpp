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


void ApplicationItemPrivate::initializePlatform()
{
    // scene update should not happen as we stick to screen size
    _q_sceneUpdate(QSize(Screen::instance()->width(), Screen::instance()->height()));
}

void ApplicationItemPrivate::platformCompleted()
{
    // the platform launches apps in Landscape mode, expected is to be in Portrait.
    // Therefore the layout is moved to portrait

    setOrientation(Screen::Portrait);
}
