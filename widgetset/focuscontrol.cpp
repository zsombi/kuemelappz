#include "focuscontrol.h"
#include "focuscontrol_p.h"
#include "checkgroup.h"
#include "widgetset.h"

#include <QtDeclarative/qdeclarative.h>
#include <QKeyEvent>
#include <QPainter>

#define TRACE_FOCUSING

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
    prev(0),
    next(0),
    first(0),
    last(0),
    parent(0),
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
  Handle parent change to add component to CheckGroup
*/
void FocusControlPrivate::_q_updateParent()
{
    Q_Q(FocusControl);
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
    // update parent sibling
    //updateParentSibling();
    updateFocusSiblings();
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

void FocusControlPrivate::_q_handleChildrenChange()
{
    if (focusType != FocusControl::FocusGroup)
        return;
    // do we have the first focusable item?
    if (lastFocusItem)
        return;
    Q_Q(FocusControl);
    QList<QGraphicsItem*> ilist = q->childItems();
#ifdef TRACE_FOCUSING
    qDebug() << __FUNCTION__ << q->objectName()<<"children.size="<< ilist.size();
#endif
    for (int i = 0; i < ilist.size(); i++) {
        FocusControl *cl = qobject_cast<FocusControl*>(ilist[i]);
        if (cl) {
#ifdef TRACE_FOCUSING
            qDebug() << "child:" << cl->objectName() << "::OBJ::"<< cl;
#endif
            if (cl && (cl->focusType() == FocusControl::Focusable)) {
                lastFocusItem = cl;
                break;
            }
        }
    }
    /*
    if (focusType != FocusControl::FocusGroup)
        return;
    Q_Q(FocusControl);
    QList<FocusControl*> clist = q->findChildren<FocusControl*>();
    foreach(FocusControl *cl, clist)
        cl->setParentItem(q);
    */
}

void FocusControlPrivate::updateParentSibling()
{
    if (focusType == FocusControl::Decorative)
        return;
    Q_Q(FocusControl);
    if (!parent) {
        QDeclarativeItem *po = q->parentItem();
        //QObject *po = q->parent();
        while (po && !parent) {
            qDebug() << "\tpo=" << po << po->objectName();
            parent = qobject_cast<FocusControl*>(po);
            if (parent && (parent->d_ptr->focusType != FocusControl::FocusGroup))
                parent = 0;
            po = po->parentItem();
            //po = po->parent();
        }
    }
#ifdef TRACE_FOCUSING
    if (!parent) {
        if (q->parent())
            qDebug() << __FUNCTION__ << q->objectName() << "::focusType"<< q->focusTypeString() << "parent()" << q->parent()->objectName();
        else
            qDebug() << __FUNCTION__ << q->objectName() << "::focusType"<< q->focusTypeString() << "parent()null";
    } else
        qDebug() << __FUNCTION__ << q->objectName() << "::focusType"<< q->focusTypeString() << "group" << parent->objectName();
#endif
}

void FocusControlPrivate::updateFocusSiblings()
{
    Q_Q(FocusControl);

    first = last = prev = next = 0;
    if (focusType == FocusControl::Decorative)
        return;
    if (!parent) {
        qWarning() << __FUNCTION__<< "Parent was not set yet!";
        updateParentSibling();
        if (!parent && !q->parentItem())
            return;
    }

    //QList<FocusControl*> children = parent->findChildren<FocusControl*>();
#ifdef TRACE_FOCUSING
    qDebug() << __FUNCTION__ << q->parentItem()->objectName();
#endif
    QDeclarativeItem *pp = (parent) ? parent : qobject_cast<QDeclarativeItem*>(q->parent());
    Q_ASSERT(pp != NULL);

    //QList<FocusControl*> children = pp->findChildren<FocusControl*>();
    QList<QGraphicsItem*> children = pp->childItems();
    const int size = children.size();
    int i;
    FocusControl *cl = 0;
    // set the first and last
    for (i = 0; (i < size) && !first; i++) {
        cl = qobject_cast<FocusControl*>(children[i]);
        if (cl && cl->d_ptr->focusType == focusType)
            first = cl;
    }
    for (i = size - 1; (i >= 0) && !last; i--) {
        cl = qobject_cast<FocusControl*>(children[i]);
        if (cl && cl->d_ptr->focusType == focusType)
            last = cl;
    }
    // set close siblings (prev and next)
    i = children.indexOf(q);
    if (i >= 0) {
        int j;
        for (j = i - 1; (j >= 0) && !prev; j--) {
            cl = qobject_cast<FocusControl*>(children[j]);
            if (cl && cl->d_ptr->focusType == focusType)
                prev = cl;
        }
        for (j = i + 1; (j < size) && !next; j++) {
            cl = qobject_cast<FocusControl*>(children[j]);
            if (cl && cl->d_ptr->focusType == focusType)
                next = cl;
        }
    }
    // update last focused item in the group
    if (parent) {
        if (focusType == FocusControl::Focusable) {
            parent->d_ptr->lastFocusItem = q;
        } else if (!lastFocusItem) {
            children = q->childItems();
            for (int i = 0; i < children.size(); i++) {
                cl = qobject_cast<FocusControl*>(children[i]);
                if (cl && cl->d_ptr->focusType == FocusControl::Focusable) {
                    lastFocusItem = cl;
                    break;
                }
            }
        }
    }
}
/*
FocusControl* FocusControlPrivate::prevFocusable(FocusControl *rel, FocusControl::FocusType focus)
{
    if (!rel) {
        Q_Q(FocusControl);
        rel = q;
    }
    //QList<FocusControl*> children = rel->parent()->findChildren<FocusControl*>();
    QList<FocusControl*> children = controlGroup()->findChildren<FocusControl*>();
    for (int i = children.indexOf(rel) - 1; i >= 0; i--) {
        FocusControl *cl = children[i];
        if (cl->d_ptr->focusType == focus) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << cl->objectName() << "parent=" << rel->parent()->objectName();
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
    //QList<FocusControl*> children = rel->parent()->findChildren<FocusControl*>();
    QList<FocusControl*> children = controlGroup()->findChildren<FocusControl*>();
    for (int i = children.indexOf(rel) + 1; i < children.size(); i++) {
        FocusControl *cl = children[i];
        if (cl->d_ptr->focusType == focus) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << cl->objectName() << "parent=" << rel->parent()->objectName();
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
    //QList<FocusControl*> children = rel->parent()->findChildren<FocusControl*>();
    QList<FocusControl*> children = controlGroup()->findChildren<FocusControl*>();
    for (int i = 0; i < children.size(); i++) {
        FocusControl *cl = children[i];
        if (cl->d_ptr->focusType == focus) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << cl->objectName() << "parent=" << rel->parent()->objectName();
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
    //QList<FocusControl*> children = rel->parent()->findChildren<FocusControl*>();
    QList<FocusControl*> children = controlGroup()->findChildren<FocusControl*>();
    for (int i = children.size() - 1; i >= 0; i--) {
        FocusControl *cl = children[i];
        if (cl->d_ptr->focusType == focus) {
#ifdef TRACE_FOCUSING
            qDebug() << __FUNCTION__ << cl->objectName() << "parent=" << rel->parent()->objectName();
#endif
            return cl;
        }
    }

    return 0;
}
*/
void FocusControlPrivate::focusGroupElement()
{
    if (focusType != FocusControl::FocusGroup)
        return;
    Q_Q(FocusControl);
    if (!lastFocusItem) {
        // set the first focusable child
        qWarning() << "No focusable set to group!";
    }
#ifdef TRACE_FOCUSING
    if (lastFocusItem)
        qDebug() << __FUNCTION__ << lastFocusItem->objectName();
    else
        qDebug() << __FUNCTION__ << lastFocusItem;
#endif
    if (lastFocusItem)
        lastFocusItem->forceActiveFocus();

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
/*
    if (d->focusType & FocusGroup) {
        // set the focus to the first styled item
        QList<FocusControl*> items = findChildren<FocusControl*>();
        if (!items.isEmpty())
            items[0]->forceActiveFocus();
    }
*/
    if (d->focusType == FocusControl::Decorative)
        return;
    // get the siblings, depending on the focus type these can be either controls or groups
    d->updateFocusSiblings();

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
        d->updateFocusSiblings();
        if (!WidgetSet::instance()->mobilePlatform()) {
            // toggle key focus
            bool toggle = (d->focusType == Focusable);
            setFlag(QGraphicsItem::ItemIsFocusable, toggle);
            //setFlag(QGraphicsItem::ItemIsPanel, toggle);
            /*
            toggle = (d->focusType == FocusGroup);
            setFlag(QGraphicsItem::ItemIsFocusScope, toggle);
            */
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

QDeclarativeListProperty<ControlAction> FocusControl::actions()
{
    Q_D(FocusControl);
    return QDeclarativeListProperty<ControlAction>(this, d->actions);
}

FocusControl *FocusControl::prevControl() const
{
    Q_D(const FocusControl);
    return d->prev;
}
FocusControl *FocusControl::nextControl() const
{
    Q_D(const FocusControl);
    return d->next;
}
FocusControl *FocusControl::groupControl() const
{
    Q_D(const FocusControl);
    return d->parent;
}

void FocusControl::focusOnNext()
{
    Q_D(FocusControl);
    if (d->next)
        d->next->forceActiveFocus();
}
void FocusControl::focusOnPrevious()
{
    Q_D(FocusControl);
    if (d->prev)
        d->prev->forceActiveFocus();
}
void FocusControl::focusOnFirst()
{
    Q_D(FocusControl);
    if (d->first)
        d->first->forceActiveFocus();
}
void FocusControl::focusOnLast()
{
    Q_D(FocusControl);
    if (d->last)
        d->last->forceActiveFocus();
}

void FocusControl::focusOnFirstGroup()
{
    Q_D(FocusControl);
    if (d->focusType == FocusGroup) {
        if (d->first)
            d->first->d_ptr->focusGroupElement();
    } else {
        // not a group, look after its parent
        if (d->parent)
            d->parent->focusOnFirstGroup();
        else // no group item, focus on first
            focusOnFirst();
    }
}
void FocusControl::focusOnLastGroup()
{
    Q_D(FocusControl);
    if (d->focusType == FocusGroup) {
        if (d->last)
            d->last->d_ptr->focusGroupElement();
    } else {
        // not a group, look after its parent
        if (d->parent)
            d->parent->focusOnLastGroup();
        else
            // no group item, focus on first
            focusOnLast();
    }
}
void FocusControl::focusOnNextGroup()
{
    Q_D(FocusControl);
    if (d->focusType == FocusGroup) {
        if (d->next)
            d->prev->d_ptr->focusGroupElement();
    } else {
        if (d->parent)
            d->parent->focusOnNextGroup();
        else
            // no group item, focus on first
            focusOnNext();
    }
}
void FocusControl::focusOnPreviousGroup()
{
    Q_D(FocusControl);
    if (d->focusType == FocusGroup) {
        if (d->prev)
            d->prev->d_ptr->focusGroupElement();
    } else {
        if (d->parent)
            d->parent->focusOnPreviousGroup();
        else
            // no group item, focus on first
            focusOnPrevious();
    }
}

#include "moc_focuscontrol.cpp"
