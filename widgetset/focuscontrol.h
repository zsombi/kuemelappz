#ifndef FOCUSCONTROL_H
#define FOCUSCONTROL_H

#include <QDeclarativeItem>

class DeclarativeAction: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int key READ key WRITE setKey NOTIFY actionChanged)
    Q_PROPERTY(Qt::KeyboardModifiers keyModifier READ keyModifier WRITE setKeyModifier NOTIFY actionChanged)
    Q_PROPERTY(QString slot READ slot WRITE setSlot NOTIFY actionChanged)
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY actionChanged)
public:
    DeclarativeAction(QObject *parent = 0);
    static DeclarativeAction *createAction(QObject *parent, int key, const QString &slot = QString(), const QVariant &data = QVariant());
protected:
    bool eventFilter(QObject *sender, QEvent *event);
Q_SIGNALS:
    void actionChanged();
    void triggered(DeclarativeAction *action);
protected:
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
    int mKey;
    Qt::KeyboardModifiers mKeyModifier;
    QString mSlot;
    QVariant mData;
};
QML_DECLARE_TYPE(DeclarativeAction)

class FocusControlPrivate;
class FocusControl : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(FocusType focusType READ focusType WRITE setFocusType NOTIFY focusTypeChanged FINAL)
    Q_PROPERTY(int controlId READ controlId WRITE setControlId NOTIFY controlIdChanged FINAL)
    Q_PROPERTY(QDeclarativeListProperty<DeclarativeAction> actions READ actions)
    Q_ENUMS(FocusType)
public:
    enum FocusType {
        Decorative,         // the item is just for decoration, does not react on key focusing
        Focusable,          // the element is focusable
        FocusGroup          // the element is a focus group (contains focusable elements)
    };
    FocusControl(QDeclarativeItem *parent = 0);
    virtual ~FocusControl();

    // returns the closest StyledItem parent
    FocusControl *focusParent(FocusType type = Focusable);
    // registers a global action object
    void registerAction(DeclarativeAction *action);

signals:
    void focusTypeChanged();
    void controlIdChanged();
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
    int controlId() const;
    void setControlId(int id);
    QDeclarativeListProperty<DeclarativeAction> actions();

private:
    Q_DECLARE_PRIVATE(FocusControl)
    QScopedPointer<FocusControlPrivate> d_ptr;
    Q_PRIVATE_SLOT(d_func(), void _q_updateParent())
    Q_PRIVATE_SLOT(d_func(), void _q_handleFocusChange(bool))
};

QML_DECLARE_TYPE(FocusControl)
QML_DECLARE_TYPE(FocusControl::FocusType)

#endif // FOCUSCONTROL_H
