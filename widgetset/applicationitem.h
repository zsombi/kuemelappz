#ifndef APPLICATIONITEM_H
#define APPLICATIONITEM_H

#include <QtDeclarative/QDeclarativeItem>
#include "styleditem.h"

class ApplicationItemPrivate;
class ApplicationItem : public StyledItem
{
    Q_OBJECT

    Q_PROPERTY(Orientation orientation READ orientation NOTIFY orientationChanged FINAL)
    Q_PROPERTY(QString orientationString READ orientationString NOTIFY orientationChanged FINAL)
    Q_PROPERTY(Orientation lockOnOrientation READ lockOnOrientation WRITE setLockOnOrientation NOTIFY orientationChanged FINAL)

    Q_ENUMS(Orientation)
public:
    enum Orientation {
        Automatic,
        Portrait,
        Landscape,
        InverseLandscape
    };

    ApplicationItem(QDeclarativeItem *parent = 0);
    ~ApplicationItem();
    
signals:

    void orientationChanged();
    
public slots:

public:
    Orientation orientation() const;
    QString orientationString() const;
    Orientation lockOnOrientation() const;
    void setLockOnOrientation(Orientation o);
protected:
    virtual void componentComplete();
private:
    Q_PRIVATE_SLOT(d_func(), void _q_rotation())
    Q_PRIVATE_SLOT(d_func(), void _q_sceneUpdate(const QSize &sceneSize))
    Q_PRIVATE_SLOT(d_func(), void _q_sensorUpdate())
    Q_DISABLE_COPY(ApplicationItem)
    Q_DECLARE_PRIVATE(ApplicationItem)
    QScopedPointer<ApplicationItemPrivate> d_ptr;
    
};

QML_DECLARE_TYPE(ApplicationItem)
QML_DECLARE_TYPE(ApplicationItem::Orientation)

#endif // APPLICATIONITEM_H
