#ifndef APPLICATIONITEM_P_H
#define APPLICATIONITEM_P_H

#include "applicationitem.h"

class QDeclarativeView;

class ApplicationItemPrivate
{
    Q_DECLARE_PUBLIC(ApplicationItem)
public:
    ApplicationItemPrivate(ApplicationItem *qq);
    ~ApplicationItemPrivate(){}

    ApplicationItem *q_ptr;

    bool initialized;
    ApplicationItem::Orientation orientation;
    bool orientationLocked;
    QDeclarativeItem *inputPanel;
// private data
    void _q_rotation();
    void _q_sceneUpdate(const QSize &sceneSize);
    void _q_sensorUpdate();
    void resize(const QSize &size);
    void setOrientation(ApplicationItem::Orientation o);
    QObject *sensor;
    QSize screenSize;
    QDeclarativeView *appView;

};

#endif // APPLICATIONITEM_P_H
