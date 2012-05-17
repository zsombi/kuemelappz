#ifndef APPLICATIONITEM_H
#define APPLICATIONITEM_H

#include <QtDeclarative/QDeclarativeItem>
#include "styleditem.h"
#include "screen.h"

class ApplicationItemPrivate;
class ApplicationItem : public StyledItem
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(Screen::Orientation orientation READ orientation NOTIFY orientationChanged FINAL)
public:

    ApplicationItem(QDeclarativeItem *parent = 0);
    ~ApplicationItem();
protected: // to finalize layout setup
    virtual void componentComplete();

Q_SIGNALS:
    void orientationChanged();
    void titleChanged();
    
public Q_SLOTS:

public:
    Screen::Orientation orientation() const;
    QString title() const;
    void setTitle(const QString &txt);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_sceneUpdate(const QSize &sceneSize))
    Q_PRIVATE_SLOT(d_func(), void _q_sensorUpdate())
    Q_DISABLE_COPY(ApplicationItem)
    Q_DECLARE_PRIVATE(ApplicationItem)
    QScopedPointer<ApplicationItemPrivate> d_ptr;
    
};

QML_DECLARE_TYPE(ApplicationItem)

#endif // APPLICATIONITEM_H
