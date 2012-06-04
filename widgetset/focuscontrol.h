#ifndef FOCUSCONTROL_H
#define FOCUSCONTROL_H

#include <QDeclarativeItem>

class FocusControl;
class ControlAction: public QObject
{
    Q_OBJECT
    Q_PROPERTY(ActionFlags actionFlags READ actionFlags WRITE setActionFlags NOTIFY actionChanged)
    Q_PROPERTY(int key READ key WRITE setKey NOTIFY actionChanged)
    Q_PROPERTY(Qt::KeyboardModifiers keyModifier READ keyModifier WRITE setKeyModifier NOTIFY actionChanged)
    Q_PROPERTY(QString slot READ slot WRITE setSlot NOTIFY actionChanged)
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY actionChanged)

    Q_FLAGS(ActionFlag ActionFlags)
public:
    enum ActionFlag {
        None = 0x00,
        Controls = 0x01,
        Groups = 0x02,
        Both = Controls | Groups
    };
    Q_DECLARE_FLAGS(ActionFlags, ActionFlag)

    ControlAction(QObject *parent = 0);
    static ControlAction *createAction(QObject *parent, int key, Qt::KeyboardModifiers kmodifier, ActionFlags flags, const QString &slot = QString(), const QVariant &data = QVariant());
protected:
    bool eventFilter(QObject *sender, QEvent *event);
Q_SIGNALS:
    void actionChanged();
    void triggered(ControlAction *action);

public: // getter/setters
    ActionFlags actionFlags() const;
    void setActionFlags(ActionFlags flags);
    int key() const;
    void setKey(int k);
    Qt::KeyboardModifiers keyModifier() const;
    void setKeyModifier(Qt::KeyboardModifiers k);
    QString slot() const;
    void setSlot(const QString &s);
    QVariant data() const;
    void setData(const QVariant& d);

private:
    bool mGlobalAction;
    ActionFlags mFlags;
    int mKey;
    Qt::KeyboardModifiers mKeyModifier;
    QString mSlot;
    QVariant mData;
};
QML_DECLARE_TYPE(ControlAction)
Q_DECLARE_OPERATORS_FOR_FLAGS(ControlAction::ActionFlags)

class FocusControlPrivate;
class FocusControl : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(FocusType focusType READ focusType WRITE setFocusType NOTIFY focusTypeChanged FINAL)
    Q_PROPERTY(QString focusTypeString READ focusTypeString NOTIFY focusTypeChanged FINAL)
    Q_PROPERTY(int controlId READ controlId WRITE setControlId NOTIFY controlIdChanged FINAL)
    Q_PROPERTY(QDeclarativeItem *attachedControl READ attachedControl WRITE setAttachedControl NOTIFY attachedControlChanged)
    Q_PROPERTY(QDeclarativeListProperty<ControlAction> actions READ actions)
    Q_PROPERTY(FocusControl *prevControl READ prevControl CONSTANT FINAL)
    Q_PROPERTY(FocusControl *nextControl READ nextControl CONSTANT FINAL)
    Q_PROPERTY(FocusControl *groupControl READ groupControl CONSTANT FINAL)
    Q_ENUMS(FocusType)
public:
    enum FocusType {
        Decorative,         // the item is just for decoration, does not react on key focusing
        Focusable,          // the element is focusable
        FocusGroup          // the element is a focus group (contains focusable elements)
    };
    FocusControl(QDeclarativeItem *parent = 0);
    virtual ~FocusControl();

    // registers a global action object
    void registerAction(ControlAction *action);

signals:
    void focusTypeChanged();
    void controlIdChanged();
    void attachedControlChanged();
    // triggered upon activation/deactivation and action invocation using keys
    void activated();
    void deactivated();
    void invoked();

public slots:
    void focusOnNext();
    void focusOnPrevious();
    void focusOnFirst();
    void focusOnLast();
    void focusOnFirstGroup();
    void focusOnLastGroup();
    void focusOnNextGroup();
    void focusOnPreviousGroup();

protected:
    virtual void componentComplete();
    // to paint hghlighted state yet
    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:
    FocusType focusType() const;
    void setFocusType(FocusType flags);
    QString focusTypeString() const;
    int controlId() const;
    void setControlId(int id);
    QDeclarativeItem *attachedControl() const;
    void setAttachedControl(QDeclarativeItem *item);
    QDeclarativeListProperty<ControlAction> actions();

    FocusControl *prevControl();
    FocusControl *nextControl();
    FocusControl *groupControl();

private:
    Q_DECLARE_PRIVATE(FocusControl)
    QScopedPointer<FocusControlPrivate> d_ptr;
    Q_PRIVATE_SLOT(d_func(), void _q_updateParent())
    Q_PRIVATE_SLOT(d_func(), void _q_handleFocusChange(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_handleChildrenChange())
};

QML_DECLARE_TYPE(FocusControl)
QML_DECLARE_TYPE(FocusControl::FocusType)

#endif // FOCUSCONTROL_H
