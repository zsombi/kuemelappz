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
    Q_PROPERTY(QObject* activeItem READ activeItem NOTIFY activeItemChanged FINAL)
    Q_PROPERTY(int activeId READ activeId WRITE setActiveId NOTIFY activeItemChanged FINAL)
    Q_PROPERTY(int itemSize READ itemSize WRITE setItemSize NOTIFY itemSizeChanged FINAL)
    Q_PROPERTY(QDeclarativeItem *groupBody READ layout WRITE setLayout NOTIFY groupBodyChanged DESIGNABLE false)
    Q_PROPERTY(BodyType bodyType READ bodyType WRITE setBodyType NOTIFY bodyTypeChanged FINAL)
    Q_PROPERTY(BodyFit bodyFit READ bodyFit WRITE setBodyFit NOTIFY bodyFitChanged FINAL)
    Q_CLASSINFO("DefaultProperty", "groupBody")
    Q_ENUMS(BodyType BodyFit)
public:
    enum BodyType {
        None = 0,
        Horizontal,
        Vertical
    };

    enum BodyFit {
        Default = 0,
        FitToGroup,
        ExpandGroup,
        Custom
    };

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
    BodyType bodyType() const;
    void setBodyType(BodyType type);
    BodyFit bodyFit() const;
    void setBodyFit(BodyFit type);

signals:
    void autoIdChanged();
    void itemSizeChanged();
    void activeItemChanged();
    void groupBodyChanged();
    void bodyTypeChanged();
    void bodyFitChanged();

public slots:
    void updateGroupItem(QObject *item);
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
QML_DECLARE_TYPE(CheckGroup::BodyType)
QML_DECLARE_TYPE(CheckGroup::BodyFit)

#endif // CHECKGROUP_H
