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
    Q_PROPERTY(Orientation lockToOrientation READ lockToOrientation WRITE setLockToOrientation NOTIFY orientationChanged FINAL)
    Q_PROPERTY(QDeclarativeItem *inputPanel READ inputPanel WRITE setInputPanel NOTIFY inputPanelChanged)

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
    void inputPanelChanged();
    
public slots:

public:
    Orientation orientation() const;
    QString orientationString() const;
    Orientation lockToOrientation() const;
    void setLockToOrientation(Orientation o);
    QDeclarativeItem *inputPanel() const;
    void setInputPanel(QDeclarativeItem *ip);
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