#ifndef CHECKGROUP_H
#define CHECKGROUP_H

#include <QDeclarativeItem>
#include <QSignalMapper>

/* To handle radio groups, page tabs, etc, anything that resembles radio button behavior.
  */
class CheckGroupPrivate;
class CheckGroup : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(CheckGroup)
    Q_PROPERTY(bool autoId READ autoId WRITE setAutoId NOTIFY autoIdChanged FINAL)
    Q_PROPERTY(bool resizeChildrenToGroup READ resizeChildrenToGroup WRITE setResizeChildrenToGroup NOTIFY resizeChildrenToGroupChanged FINAL)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged FINAL)
    Q_PROPERTY(int itemSize READ itemSize WRITE setItemSize NOTIFY itemSizeChanged FINAL)
    Q_PROPERTY(int activeId READ activeId WRITE setActiveId NOTIFY activeItemChanged FINAL)
    Q_PROPERTY(QObject* activeItem READ activeItem NOTIFY activeItemChanged FINAL)
    Q_PROPERTY(QDeclarativeItem *groupBody READ layout WRITE setLayout NOTIFY groupBodyChanged DESIGNABLE false)
    Q_CLASSINFO("DefaultProperty", "groupBody")
public:

    explicit CheckGroup(QDeclarativeItem *parent = 0);
    virtual ~CheckGroup();
protected:
    virtual void componentComplete();

private: //getter/setter
    bool autoId();
    void setAutoId(bool value);
    int itemSize() const;
    void setItemSize(int size);
    QObject *activeItem() const;
    int activeId() const;
    void setActiveId(int id);
    QDeclarativeItem *layout() const;
    void setLayout(QDeclarativeItem *item);
    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation o);
    bool resizeChildrenToGroup() const;
    void setResizeChildrenToGroup(bool v);

signals:
    void autoIdChanged();
    void itemSizeChanged();
    void activeItemChanged();
    void groupBodyChanged();
    void orientationChanged();
    void resizeChildrenToGroupChanged();

public slots:
    void addGroupItem(QObject *item);
    void removeGroupItem(QObject *item);
    void adjustLayout();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_uncheckPrev(int))
    Q_PRIVATE_SLOT(d_func(), void _q_updateParent())
    Q_PRIVATE_SLOT(d_func(), void _q_updateGroupItems())

private:
    Q_DECLARE_PRIVATE(CheckGroup)
    QScopedPointer<CheckGroupPrivate> d_ptr;
};

QML_DECLARE_TYPE(CheckGroup)

#endif // CHECKGROUP_H
