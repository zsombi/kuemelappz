#include "focuscontrol.h"
#include "focuscontrol_p.h"
#include "checkgroup.h"
#include "widgetset.h"

#include <QtDeclarative/qdeclarative.h>
#include <QKeyEvent>
#include <QPainter>

#define TRACE_FOCUSING

DeclarativeAction::DeclarativeAction(QObject *parent) :
    QObject(parent),
    mGlobalAction(false),
    mKey(0)
{
}
/**
  creates a global action; global actions are the ones that upon invocation do
  not emit any signal, but invoke the current focus object's slot.
  */
DeclarativeAction *DeclarativeAction::createAction(QObject *parent, int key, const QString &slot, const QVariant &data)
{
    DeclarativeAction *ret = new DeclarativeAction(parent);
    ret->mGlobalAction = true;
    ret->mKey = key;
    ret->mSlot = slot;
    ret->mData = data;
    // register the action to the parent (should be the root)
    FocusControl *root = qobject_cast<FocusControl*>(parent);
    if (root)
        root->registerAction(ret);
    return ret;
}

/**
  performs action on the given host, either by emiting action's signal (local actions)
  or by invoking host's slot.
  */
bool DeclarativeAction::eventFilter(QObject *host, QEvent *event)
{
    bool ret = false;
    QKeyEvent *eKey = static_cast<QKeyEvent*>(event);
    if ((event->type() == QEvent::KeyPress) && (eKey->key() == mKey) && (eKey->modifiers() == mKeyModifier)) {
        if (mGlobalAction) {
#ifdef TRACE_FOCUSING
            qDebug() <<"global-action"<< ",host" << host->objectName();
#endif
            QMetaObject::invokeMethod(host, mSlot.toAscii());
        } else {
#ifdef TRACE_FOCUSING
            qDebug() <<"local-action"<< ",host" << host->objectName();
#endif
            emit triggered(this);
        }
        // accept event
        ret = true;
    } else {
        ret = QObject::eventFilter(host, event);
    }
    return ret;
}

int DeclarativeAction::key() const
{
    return mKey;
}
void DeclarativeAction::setKey(int k)
{
    if (k != mKey) {
        mKey = k;
        emit actionChanged();
    }
}
Qt::KeyboardModifiers DeclarativeAction::keyModifier() const
{
    return mKeyModifier;
}
void DeclarativeAction::setKeyModifier(Qt::KeyboardModifiers k)
{
    if (k != mKeyModifier) {
        mKeyModifier = k;
        emit actionChanged();
    }
}


QString DeclarativeAction::slot() const
{
    return mSlot;
}
void DeclarativeAction::setSlot(const QString &s)
{
    if (s != mSlot) {
        mSlot = s;
        emit actionChanged();
    }
}
QVariant DeclarativeAction::data() const
{
    return mData;
}
void DeclarativeAction::setData(const QVariant& d)
{
    if (d != mData) {
        mData = d;
        emit actionChanged();
    }
}
/*=====================================================================================================================================
  =====================================================================================================================================*/

FocusControlPrivate::FocusControlPrivate(FocusControl *qq) :
    q_ptr(qq),
    focusType(FocusControl::Decorative),
    group(0),
    controlId(-1),
    focusableControl(0)
{
    // connect parentChanged() to add component to CheckGroup
    QObject::connect(q_ptr, SIGNAL(parentChanged()), q_ptr, SLOT(_q_updateParent()));
    // this may not be needed after all...
    QObject::connect(q_ptr, SIGNAL(focusChanged(bool)), q_ptr, SLOT(_q_handleFocusChange(bool)));
}
FocusControlPrivate::~FocusControlPrivate()
{
    QObject::disconnect(q_ptr, SIGNAL(parentChanged()), q_ptr, SLOT(_q_updateParent()));
    QObject::disconnect(q_ptr, SIGNAL(focusChanged(bool)), q_ptr, SLOT(_q_handleFocusChange(bool)));
}

/*
  Handle parent change to add component to CheckGroup
*/
void FocusControlPrivate::_q_updateParent()
{
    Q_Q(FocusControl);
    QDeclarativeItem *parent = q->parentItem();
    if (!parent || (parent && !parent->parentItem()))
        return;
    // dirrect parent can be either CheckGroup or a positioner element
    group = qobject_cast<CheckGroup*>(parent);
    if (!group)
        group = qobject_cast<CheckGroup*>(parent->parentItem());
    // add item to group, group will check the rest
    if (group)
        group->addGroupItem(q);
}

void FocusControlPrivate::_q_handleFocusChange(bool f)
{
#ifdef TRACE_FOCUSING
    if (q_ptr->objectName().isEmpty())
        qDebug() << __FUNCTION__ << q_ptr << "focus=" << f;
    else
        qDebug() << __FUNCTION__ << q_ptr->objectName() << "focus=" << f;
#endif
    Q_Q(FocusControl);
    if (focusableControl && (focusableControl != q))
        focusableControl->setFocus(f);
    if (f) {
        focusGroupElement();
        emit q->activated();
    } else
        emit q->deactivated();
}


FocusControl* FocusControlPrivate::prevFocusable(FocusControl *rel, FocusControl::FocusType focus)
{
    if (!rel) {
        Q_Q(FocusControl);
        rel = q;
    }
    QList<FocusControl*> children = rel->parent()->findChildren<FocusControl*>();
    for (int i = children.indexOf(rel) - 1; i >= 0; i--) {
        FocusControl *cl = children[i];
        if (cl->focusType() == focus) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << cl;
#endif
            return cl;
        }
    }
    return 0;
}

FocusControl* FocusControlPrivate::nextFocusable(FocusControl *rel, FocusControl::FocusType focus)
{
    if (!rel) {
        Q_Q(FocusControl);
        rel = q;
    }
    QList<FocusControl*> children = rel->parent()->findChildren<FocusControl*>();
    for (int i = children.indexOf(rel) + 1; i < children.size(); i++) {
        FocusControl *cl = children[i];
        if (cl->focusType() == focus) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << cl;
#endif
            return cl;
        }
    }
    return 0;
}
FocusControl* FocusControlPrivate::firstFocusable(FocusControl *rel, FocusControl::FocusType focus)
{
    if (!rel) {
        Q_Q(FocusControl);
        rel = q;
    }
    QList<FocusControl*> children = rel->parent()->findChildren<FocusControl*>();
    for (int i = 0; i < children.size(); i++) {
        FocusControl *cl = children[i];
        if (cl->focusType() == focus) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << cl;
#endif
            return cl;
        }
    }

    return 0;
}

FocusControl* FocusControlPrivate::lastFocusable(FocusControl *rel, FocusControl::FocusType focus)
{
    if (!rel) {
        Q_Q(FocusControl);
        rel = q;
    }
    QList<FocusControl*> children = rel->parent()->findChildren<FocusControl*>();
    for (int i = children.size() - 1; i >= 0; i--) {
        FocusControl *cl = children[i];
        if (cl->focusType() & focus) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << cl;
#endif
            return cl;
        }
    }

    return 0;
}

void FocusControlPrivate::focusGroupElement()
{
    if (focusType != FocusControl::FocusGroup)
        return;
    Q_Q(FocusControl);
    if (!lastFocusItem) {
        // set the first focusable child
        QList<FocusControl*> children = q->findChildren<FocusControl*>();
        for (int i = 0; i < children.size(); i++) {
            FocusControl *cl = children[i];
            if (cl->focusType() == FocusControl::Focusable) {
                lastFocusItem = cl;
                break;
            }
        }
    }
#ifdef TRACE_FOCUSING
    qDebug() << __FUNCTION__ << lastFocusItem;
#endif
    lastFocusItem->forceActiveFocus();

}

/*====================================================================================
  ====================================================================================*/

FocusControl::FocusControl(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    d_ptr(new FocusControlPrivate(this))
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:
    setFlag(ItemHasNoContents, false);

}

FocusControl::~FocusControl()
{}

/**
  Returns the closest parent item that is derived from FocusControl
  */
FocusControl *FocusControl::focusParent(FocusType type)
{
    FocusControl *ret = 0;

    QDeclarativeItem *pi = parentItem();
    while (pi && !ret) {
        ret = qobject_cast<FocusControl*>(pi);
        if (ret && (ret->focusType() != type))
            ret = 0;
        pi = pi->parentItem();
    }
    return ret;
}


void FocusControl::registerAction(DeclarativeAction *action)
{
    if (!action)
        return;
    Q_D(FocusControl);
    d->actions << action;
}

void FocusControl::componentComplete()
{
    QDeclarativeItem::componentComplete();
    Q_D(FocusControl);
/*
    if (d->focusType & FocusGroup) {
        // set the focus to the first styled item
        QList<FocusControl*> items = findChildren<FocusControl*>();
        if (!items.isEmpty())
            items[0]->forceActiveFocus();
    }
*/
    // check if there are no actions defined, set the root ones as filters
    if (d->focusType == Focusable) {
        if (d->actions.isEmpty()) {
            FocusControl *root = qobject_cast<FocusControl*>(WidgetSet::instance()->appWindow);
            foreach(DeclarativeAction *a, root->d_ptr->actions) {
                installEventFilter(a);
            }
        } else {
            // set actions as event filters
            foreach(DeclarativeAction *a, d->actions) {
                installEventFilter(a);
            }
        }
    }
}

// paint a frame around the focus item, yet for testing purposes as each focusable item
// should have its own highlighted style
void FocusControl::paint(QPainter *painter, const QStyleOptionGraphicsItem *gi, QWidget *w)
{
    Q_UNUSED(gi)
    Q_UNUSED(w)
    Q_D(FocusControl);

    if (!hasFocus())
        return;

    QSize rectSize(width(), height());
    if (rectSize.isEmpty())
        return;
    QRect rect(QPoint(0,0), rectSize);

// TODO: get the color and radius from the style
    QColor color("red");

    painter->setBrush(QBrush(color));
    painter->setPen(QPen(color));

    bool oldAA = painter->testRenderHint(QPainter::Antialiasing);
    bool oldSmooth = painter->testRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter->drawRoundedRect(rect, 10, 10);

    painter->setRenderHint(QPainter::Antialiasing, oldAA);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, oldSmooth);
}

FocusControl::FocusType FocusControl::focusType() const
{
    Q_D(const FocusControl);
    return d->focusType;
}
void FocusControl::setFocusType(FocusControl::FocusType type)
{
    Q_D(FocusControl);
    if (d->focusType != type) {
        d->focusType = type;
        bool toggle = (d->focusType == Focusable);
        setFlag(QGraphicsItem::ItemIsFocusable, toggle);
        //setFlag(QGraphicsItem::ItemIsPanel, toggle);
        /*
        toggle = (d->focusType == FocusGroup);
        setFlag(QGraphicsItem::ItemIsFocusScope, toggle);
        */
        emit focusTypeChanged();
    }
}

int FocusControl::controlId() const
{
    Q_D(const FocusControl);
    return d->controlId;
}
void FocusControl::setControlId(int id)
{
    Q_D(FocusControl);
    if (d->controlId != id) {
        d->controlId = id;
        emit controlIdChanged();
    }
}

QDeclarativeListProperty<DeclarativeAction> FocusControl::actions()
{
    Q_D(FocusControl);
    return QDeclarativeListProperty<DeclarativeAction>(this, d->actions);
}


void FocusControl::focusOnNext()
{
    Q_D(FocusControl);

    FocusControl * next = d->nextFocusable(0);
    if (next) {
        // emit defocusing for the current one
        next->forceActiveFocus();
    }
}
void FocusControl::focusOnPrevious()
{
    Q_D(FocusControl);
    FocusControl * prev = d->prevFocusable(0);
    if (prev)
        prev->forceActiveFocus();
}
void FocusControl::focusOnFirst()
{
    Q_D(FocusControl);
    FocusControl * first = d->firstFocusable();
    if (first)
        first->forceActiveFocus();
}
void FocusControl::focusOnLast()
{
    Q_D(FocusControl);
    FocusControl * last = d->lastFocusable();
    if (last)
        last->forceActiveFocus();
}

void FocusControl::focusOnFirstGroup()
{
    Q_D(FocusControl);
    if (d->focusType == FocusGroup) {
        FocusControl *first = d->firstFocusable(0, FocusGroup);
        if (first)
            first->forceActiveFocus();
    } else {
        // not a group, look after its parent
        FocusControl *group = focusParent(FocusGroup);
        if (group)
            group->focusOnFirstGroup();
        else // no group item, focus on first
            focusOnFirst();
    }
}
void FocusControl::focusOnLastGroup()
{
    Q_D(FocusControl);
    if (d->focusType == FocusGroup) {
        FocusControl *last = d->lastFocusable(0, FocusGroup);
        if (last)
            last->forceActiveFocus();
    } else {
        // not a group, look after its parent
        FocusControl *group = focusParent(FocusGroup);
        if (group)
            group->focusOnLastGroup();
        else
            // no group item, focus on first
            focusOnLast();
    }
}
void FocusControl::focusOnNextGroup()
{
    Q_D(FocusControl);
    if (d->focusType == FocusGroup) {
        FocusControl *next = d->nextFocusable(this, FocusGroup);
        if (next)
            next->forceActiveFocus();
    } else {
        // not a group, look after its parent
        FocusControl *group = focusParent(FocusGroup);
        if (group)
            group->focusOnNextGroup();
        else
            // no group item, focus on first
            focusOnNext();
    }
}
void FocusControl::focusOnPreviousGroup()
{
    Q_D(FocusControl);
    if (d->focusType == FocusGroup) {
        FocusControl *prev = d->prevFocusable(this, FocusGroup);
        if (prev)
            prev->forceActiveFocus();
    } else {
        // not a group, look after its parent
        FocusControl *group = focusParent(FocusGroup);
        if (group)
            group->focusOnPreviousGroup();
        else
            // no group item, focus on first
            focusOnPrevious();
    }
}

#include "moc_focuscontrol.cpp"
