#include "focuscontrol.h"
#include "focuscontrol_p.h"
#include "checkgroup.h"
#include "widgetset.h"

#include <QtDeclarative/qdeclarative.h>
#include <QKeyEvent>
#include <QPainter>

//#define TRACE_FOCUSING

typedef QDeclarativeListProperty<QGraphicsObject> ChildrenPropertyType;

ControlAction::ControlAction(QObject *parent) :
    QObject(parent),
    mGlobalAction(false),
    mFlags(Both),
    mKey(0),
    mKeyModifier(Qt::NoModifier)
{
}
/**
  creates a global action; global actions are the ones that upon invocation do
  not emit any signal, but invoke the current focus object's slot.
  */
ControlAction *ControlAction::createAction(QObject *parent, int key, Qt::KeyboardModifiers kmodifier, ActionFlags flags, const QString &slot, const QVariant &data)
{
    ControlAction *ret = new ControlAction(parent);
    ret->mGlobalAction = true;
    ret->mFlags = flags;
    ret->mKey = key;
    ret->mKeyModifier = kmodifier;
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
bool ControlAction::eventFilter(QObject *host, QEvent *event)
{
    bool ret = false;
    QKeyEvent *eKey = static_cast<QKeyEvent*>(event);
    if ((event->type() == QEvent::KeyPress) && (eKey->key() == mKey) && (eKey->modifiers() == mKeyModifier)) {
        if (mGlobalAction) {
#ifdef TRACE_FOCUSING
            qDebug() <<"global-action"<< "host" << host->objectName() << "slot::" << mSlot;
#endif
            QMetaObject::invokeMethod(host, mSlot.toAscii());
        } else {
#ifdef TRACE_FOCUSING
            qDebug() <<"local-action"<< "host" << host->objectName();
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

ControlAction::ActionFlags ControlAction::actionFlags() const
{
    return mFlags;
}
void ControlAction::setActionFlags(ControlAction::ActionFlags flags)
{
    if (mFlags != flags) {
        mFlags = flags;
        emit actionChanged();
    }
}

int ControlAction::key() const
{
    return mKey;
}
void ControlAction::setKey(int k)
{
    if (k != mKey) {
        mKey = k;
        emit actionChanged();
    }
}
Qt::KeyboardModifiers ControlAction::keyModifier() const
{
    return mKeyModifier;
}
void ControlAction::setKeyModifier(Qt::KeyboardModifiers k)
{
    if (k != mKeyModifier) {
        mKeyModifier = k;
        emit actionChanged();
    }
}


QString ControlAction::slot() const
{
    return mSlot;
}
void ControlAction::setSlot(const QString &s)
{
    if (s != mSlot) {
        mSlot = s;
        emit actionChanged();
    }
}
QVariant ControlAction::data() const
{
    return mData;
}
void ControlAction::setData(const QVariant& d)
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
    controlId(-1),
    focusableControl(0),
    lastFocusItem(0),
    parentSibling(0),
    group(0)
{
    // connect parentChanged() to add component to CheckGroup
    QObject::connect(q_ptr, SIGNAL(parentChanged()), q_ptr, SLOT(_q_updateParent()));
    // this may not be needed after all...
    QObject::connect(q_ptr, SIGNAL(focusChanged(bool)), q_ptr, SLOT(_q_handleFocusChange(bool)));
    // capture children changes to reparent grouped controls
    QObject::connect(q_ptr, SIGNAL(childrenChanged()), q_ptr, SLOT(_q_handleChildrenChange()));
}
FocusControlPrivate::~FocusControlPrivate()
{
    QObject::disconnect(q_ptr, SIGNAL(childrenChanged()), q_ptr, SLOT(_q_handleChildrenChange()));
    QObject::disconnect(q_ptr, SIGNAL(parentChanged()), q_ptr, SLOT(_q_updateParent()));
    QObject::disconnect(q_ptr, SIGNAL(focusChanged(bool)), q_ptr, SLOT(_q_handleFocusChange(bool)));
}

/*
  Handle parent change to add component to CheckGroup, or to update its closest FocusControl
  parent sibling.
*/
void FocusControlPrivate::_q_updateParent()
{
    Q_Q(FocusControl);

    FocusControl *parent = focusSibling(Parent);
    // check if the control had already a parent sibling
    if (parentSibling && (parentSibling != parent)) {
        // remove from that parent!
        parentSibling->d_ptr->controlList.removeAll(q);
        // add to the new parent
        parent->d_ptr->addControlToParent();
    }
    if (parentSibling != parent)
        parentSibling = parent;

    // check if the parent's last focus item can be set...
    if (parent && !parent->d_ptr->lastFocusItem)
        parent->d_ptr->lastFocusItem = q;
    // check if the control belongs to a CheckGroup, if yes add it to the group
    QDeclarativeItem *po = q->parentItem();
    if (!po || (po && !po->parentItem()))
        return;
    // dirrect parent can be either CheckGroup or a positioner element
    if (group)
        group->removeGroupItem(q);
    group = qobject_cast<CheckGroup*>(po);
    if (!group)
        group = qobject_cast<CheckGroup*>(po->parentItem());
    // add item to group, group will check the rest
    if (group)
        group->addGroupItem(q);
}

/*
  Handle focus changes, updates last focused element for a focus group
  */
void FocusControlPrivate::_q_handleFocusChange(bool focused)
{
#ifdef TRACE_FOCUSING
    qDebug() << __FUNCTION__ << q_ptr->objectName() << "focus=" << focused;
#endif
    Q_Q(FocusControl);
    // if the focus is on group, propagate the focus to it's last focused element!
    if (focusType == FocusControl::FocusGroup) {
        if (focused) {
            if (lastFocusItem)
                lastFocusItem->forceActiveFocus();
            else
                qWarning() << "WARNING!" << __FUNCTION__ << "no lastFocusItem is set for the group!";
        }
    } else if (focusType == FocusControl::Focusable) {
        // focus on the element and update parent's lastFocusItem
        FocusControl *parent = focusSibling(Parent);
        if (focused && parent) {
            parent->d_ptr->lastFocusItem = q;
        }
        // if there is a separate focusable element, then activate that (e.g. in edit control case)
        // this may screw the focus flow...
        if (focusableControl && (focusableControl != q))
            focusableControl->setFocus(focused);
    } else {
        qWarning() << "UUPS!!! FOCUS ON A DECORATIVE ELEMENT??!";
    }
    if (focused) {
        emit q->activated();
    } else
        emit q->deactivated();
}

/*
  Set first focusable item for groups
  */
void FocusControlPrivate::_q_handleChildrenChange()
{
    if (focusType != FocusControl::FocusGroup)
        return;
    // do we have the first focusable item?
    if (lastFocusItem)
        return;
    Q_Q(FocusControl);
    QList<QGraphicsItem*> children = q->childItems();
#ifdef TRACE_FOCUSING
    qDebug() << __FUNCTION__ << "object:" << q->objectName() << ", items=" << children.size();
#endif
    for (int i = 0; i < children.size(); i++) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << "child:" << children[i];
#endif
        FocusControl *cl = qobject_cast<FocusControl*>(children[i]);
        if (cl) {
            if (cl && (cl->focusType() != FocusControl::Decorative)) {
                lastFocusItem = cl;
                break;
            }
        }
    }
}

void FocusControlPrivate::addControlToParent()
{
    Q_Q(FocusControl);
    FocusControl *parent = focusSibling(Parent);
    if (parent && (parent->d_ptr->controlList.indexOf(q) < 0)) {
        parent->d_ptr->controlList << q;
    }
}

FocusControl *FocusControlPrivate::focusSibling(SiblingType siblingType, FocusControl *relativeTo)
{
    Q_Q(FocusControl);
    FocusControl *ret = 0;
    int idx;
    if (!relativeTo)
        relativeTo = q;

#ifdef TRACE_FOCUSING
    QString dbg("%1() - object %4, %3 of %2");
    dbg = dbg.arg(__FUNCTION__).arg(q->objectName());
#endif

    switch (siblingType) {
        // previous sibling control of a type
        case Previous: {
            #ifdef TRACE_FOCUSING
            dbg = dbg.arg("Previous");
            #endif
            FocusControl *parentControl = focusSibling(Parent);
            if (parentControl) {
                idx = parentControl->d_ptr->controlList.indexOf(relativeTo);
                idx--;
                if (idx >= 0)
                    ret = parentControl->d_ptr->controlList[idx];
            }
        } break;
        // next sibling control of a type
        case Next: {
            #ifdef TRACE_FOCUSING
            dbg = dbg.arg("Next");
            #endif
            FocusControl *parentControl = focusSibling(Parent);
            if (parentControl) {
                idx = parentControl->d_ptr->controlList.indexOf(relativeTo);
                idx++;
                if (idx < parentControl->d_ptr->controlList.size())
                    ret = parentControl->d_ptr->controlList[idx];
            }
        } break;
        // first sibling control of a type
        case First: {
            #ifdef TRACE_FOCUSING
            dbg = dbg.arg("First");
            #endif
            FocusControl *parentControl = focusSibling(Parent);
            if (parentControl && (parentControl->d_ptr->controlList.size() > 0)) {
                ret = parentControl->d_ptr->controlList.first();
            }
        } break;
        // last sibling control of a type
        case Last: {
            #ifdef TRACE_FOCUSING
            dbg = dbg.arg("Last");
            #endif
            FocusControl *parentControl = focusSibling(Parent);
            if (parentControl && (parentControl->d_ptr->controlList.size() > 0)) {
                ret = parentControl->d_ptr->controlList.last();
            }
        } break;
        // parent sibling control, always looks after group controls
        case Parent: {
            #ifdef TRACE_FOCUSING
            dbg = dbg.arg("Parent");
            #endif
            QDeclarativeItem *parent = q->parentItem();
            while (parent && !ret) {
                ret = qobject_cast<FocusControl*>(parent);
                if (ret && (ret->d_ptr->focusType != FocusControl::FocusGroup))
                    ret = 0;
                parent = parent->parentItem();
            }
        } break;
        // root control, always looks after focus groups
        case Root: {
            #ifdef TRACE_FOCUSING
            dbg = dbg.arg("Root");
            #endif
            QDeclarativeItem *parent = q->parentItem();
            FocusControl *tmp = 0;
            while (parent) {
                tmp = qobject_cast<FocusControl*>(parent);
                if (tmp && (tmp->d_ptr->focusType == FocusControl::FocusGroup))
                    ret = tmp;
                parent = parent->parentItem();
            }
        } break;
        // current focus item of a group
        case FocusItem: {
            #ifdef TRACE_FOCUSING
            dbg = dbg.arg("FocusItem");
            #endif
            if (focusType == FocusControl::FocusGroup)
                ret = lastFocusItem;
        } break;
    }

#ifdef TRACE_FOCUSING
    if (ret)
        dbg = dbg.arg(ret->objectName());
    else
        dbg = dbg.arg("null");
    qDebug() << dbg;
#endif
    return ret;
}

/**
  Test whether a control is eligible for the action or not.
  */
bool FocusControlPrivate::testAction(ControlAction::ActionFlags flags)
{
    if ((focusType == FocusControl::Focusable) && (flags & ControlAction::Controls))
        return true;
    if ((focusType == FocusControl::FocusGroup) && (flags & ControlAction::Groups))
        return true;
    return false;
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


void FocusControl::registerAction(ControlAction *action)
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
    if (d->focusType == FocusControl::Decorative)
        return;
#ifdef TRACE_FOCUSING
    qDebug() << __FUNCTION__ << ">>";
#endif

    // register control at its parent
    d->addControlToParent();
    // update group's lastFocusItem if not set yet
    FocusControl *parent = d->focusSibling(FocusControlPrivate::Parent);
    if (parent && !parent->d_ptr->lastFocusItem)
        parent->d_ptr->lastFocusItem = this;

    // check if there are no actions defined, set the root ones as event filters
    if (d->focusType != Decorative) {
        QList<ControlAction*> list = d->actions;
        if (d->actions.isEmpty()) {
            FocusControl *root = qobject_cast<FocusControl*>(WidgetSet::instance()->appWindow);
            list = root->d_ptr->actions;
        }
        foreach(ControlAction *a, list) {
            if (d->testAction(a->actionFlags()))
                installEventFilter(a);
        }
    }
#ifdef TRACE_FOCUSING
    qDebug() << __FUNCTION__ << "<<";
#endif
}

// paint a frame around the focus item, yet for testing purposes as each focusable item
// should have its own highlighted style
void FocusControl::paint(QPainter *painter, const QStyleOptionGraphicsItem *gi, QWidget *w)
{
    Q_UNUSED(gi)
    Q_UNUSED(w)
    Q_D(FocusControl);

    if (d->focusType != Focusable)
        return;
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
QString FocusControl::focusTypeString() const
{
    Q_D(const FocusControl);
    int index = metaObject()->indexOfEnumerator("FocusType");
    Q_ASSERT(index != -1);
    QMetaEnum enumerator = metaObject()->enumerator(index);
    return QLatin1String(enumerator.valueToKey(d->focusType));
}

void FocusControl::setFocusType(FocusControl::FocusType type)
{
    Q_D(FocusControl);
    if (d->focusType != type) {
        d->focusType = type;
        // update tab order
        if (!WidgetSet::instance()->mobilePlatform()) {
            // toggle key focus
            bool toggle = (d->focusType != Decorative);
            setFlag(QGraphicsItem::ItemIsFocusable, toggle);
            //setFlag(QGraphicsItem::ItemIsPanel, toggle);
        }
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
QDeclarativeItem *FocusControl::attachedControl() const
{
    Q_D(const FocusControl);
    return d->focusableControl;
}
void FocusControl::setAttachedControl(QDeclarativeItem *item)
{
    Q_D(FocusControl);
    if (d->focusableControl != item) {
        d->focusableControl = item;
        emit attachedControlChanged();
    }
}

QDeclarativeListProperty<ControlAction> FocusControl::actions()
{
    Q_D(FocusControl);
    return QDeclarativeListProperty<ControlAction>(this, d->actions);
}

FocusControl *FocusControl::prevControl()
{
    Q_D(FocusControl);
    return d->focusSibling(FocusControlPrivate::Previous);
}
FocusControl *FocusControl::nextControl()
{
    Q_D(FocusControl);
    return d->focusSibling(FocusControlPrivate::Next);
}
FocusControl *FocusControl::groupControl()
{
    Q_D(FocusControl);
    return d->focusSibling(FocusControlPrivate::Parent);
}

void FocusControl::focusOnNext()
{
    FocusControl *next = nextControl();
    if (next)
        next->forceActiveFocus();
}
void FocusControl::focusOnPrevious()
{
    FocusControl *prev = prevControl();
    if (prev)
        prev->forceActiveFocus();
}
void FocusControl::focusOnFirst()
{
    Q_D(FocusControl);
    FocusControl *first = d->focusSibling(FocusControlPrivate::First);
    if (first)
        first->forceActiveFocus();
}
void FocusControl::focusOnLast()
{
    Q_D(FocusControl);
    FocusControl *last = d->focusSibling(FocusControlPrivate::Last);
    if (last)
        last->forceActiveFocus();
}

void FocusControl::focusOnFirstGroup()
{
    FocusControl *parent = groupControl();
    if (parent)
        parent->focusOnFirst();
    else // no group item, focus on first
        focusOnFirst();
}
void FocusControl::focusOnLastGroup()
{
    FocusControl *parent = groupControl();
    if (parent)
        parent->focusOnLast();
    else // no group item, focus on first
        focusOnLast();
}
void FocusControl::focusOnNextGroup()
{
    FocusControl *nextGroup = groupControl();
    if (nextGroup)
        nextGroup = nextGroup->nextControl();
    if (nextGroup)
        nextGroup->forceActiveFocus();
    else
        focusOnNext();
}
void FocusControl::focusOnPreviousGroup()
{
    FocusControl *prevGroup = groupControl();
    if (prevGroup)
        prevGroup = prevGroup->prevControl();
    if (prevGroup)
        prevGroup->forceActiveFocus();
    else
        focusOnPrevious();
}

#include "moc_focuscontrol.cpp"
