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

protected: // platform specific APIs
    void initializePlatform();
    void platformResize(const QSize &sceneSize);
    void platformCompleted();
    // generic API for all mobile platforms
public: // slot to catch sensor activity, generic for mobile
    void _q_sensorUpdate();
public: // members
    ApplicationItem *q_ptr;

    bool initialized;
// private data
    void _q_sceneUpdate(const QSize &sceneSize);
    void resize(const QSize &size);
    void setOrientation(Screen::Orientation o);
    QSize screenSize;
    QDeclarativeView *appView;

};

#endif // APPLICATIONITEM_P_H
