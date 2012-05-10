/**
  ApplicatioItemPrivate desktop implementation.
  */
#include "globaldefs.h"
#include "applicationitem.h"
#include "applicationitem_p.h"
#include "widgetset.h"
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeProperty>
#include "screen.h"


void ApplicationItemPrivate::initializePlatform()
{
#ifdef Q_WS_SIMULATOR
    // we need this for simulator support
    _q_sceneUpdate(QSize(Screen::instance()->width(), Screen::instance()->height()));
#endif
}

/*******************************************************************************
  Sensor interfaces
 */
void ApplicationItemPrivate::platformCompleted()
{
    // do nothing
}
