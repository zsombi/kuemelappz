/**
  CheckGroup - element grouping radion buttons.

  Properties:

    property autoId: bool
        Specifies whether CheckGroup should give identifiers to radio items or not.

    proeprty resizeChildrenToGroup: bool
        Specifies whether group should resize each children element to fill the
        group or not.

    property orientation: Qt::Orientation
        Specifies the group children orientation, the default is horizontal.

    property itemSize: int
        Specifies the item size. The value is considered when resizeChildrenToGroup
        is true.

    property activeId: int
        Specifies the active radio component's ID.

    property activeItem: Item read-only
        Specifies the active radio component.

    default property groupBody: Item
        Default property holding the group's body component.

  Slots:
    addGroupItem(item Item)
        Adds a styled item to the group if the item is a radio component.

    adjustLayout()
        The method recalculates item size and adjusts the layout.

  */
#include "checkgroup.h"
#include "checkgroup_p.h"
#include "styleditem.h"
#include <QDeclarativeProperty>

//#define TRACE_GROUP
#ifdef TRACE_GROUP
#include <QDebug>
#endif

#define RADIO_ITEM  "_q_radioMapped"

CheckGroupPrivate::CheckGroupPrivate(CheckGroup *qq) :
    q_ptr(qq),
    signalMap(new QSignalMapper(q_ptr)),
    prevItem(0),
    activeItem(0),
    layout(0),
    orientation(Qt::Horizontal),
    activeItemId(-1),
    m_autoId(false),
    itemSize(0), itemSpacing(0),
    bodyFit(false),
    lastId(-1)
{
    q_ptr->connect(signalMap, SIGNAL(mapped(int)), SLOT(_q_uncheckPrev(int)));
}

void CheckGroupPrivate::_q_uncheckPrev(int id)
{
    //Q_Q(CheckGroup);
    // test if ID is the checked one
    QObject *item = signalMap->mapping(id);
    handleRadio(item);
}

void CheckGroupPrivate::_q_updateParent()
{
    Q_Q(CheckGroup);
#ifdef TRACE_GROUP
    qDebug() << "CheckGroupPrivate::" << __FUNCTION__ << "- parent: " << q->parent();
#endif
    if (q->parentItem())
        q->connect(q->parentItem(), SIGNAL(childrenChanged()), SLOT(_q_updateGroupItems()));
}

void CheckGroupPrivate::_q_updateGroupItems()
{
    if (!layout)
        return;
    calculateItemSize();
    QList<QGraphicsItem*> children = layout->childItems();
    int childCount = children.length();
    int i = 0, offs = 0;
    foreach (QGraphicsItem *child, children) {
        StyledItem *uiItem = qobject_cast<StyledItem*>(child);
        if (uiItem) {
            i++;
            if (itemSize > 0) {
                switch (orientation) {
                case Qt::Horizontal:
                    QDeclarativeProperty::write(uiItem, "width", itemSize);
                    QDeclarativeProperty::write(uiItem, "x", offs);
                    break;
                case Qt::Vertical:
                    QDeclarativeProperty::write(uiItem, "height", itemSize);
                    QDeclarativeProperty::write(uiItem, "y", offs);
                    break;
                default:
                    break;
                }
                offs += itemSize;

            }
        } else
            childCount--;
    }
}

void CheckGroupPrivate::calculateItemSize()
{
    if (!bodyFit || !layout)
        return;

    // calculate element sizes so they fit to the group
    QList<StyledItem*> items = layout->findChildren<StyledItem*>();
    if (items.isEmpty())
        return;

    Q_Q(CheckGroup);
    switch (orientation) {
    case Qt::Horizontal:
        itemSize = q->width() / items.count();
        break;
    case Qt::Vertical:
        itemSize = q->height() / items.count();
        break;
    default:
        break;
    }
#ifdef TRACE_GROUP
    qDebug() << __FUNCTION__ << "- itemSize =" << itemSize;
#endif
}

void CheckGroupPrivate::handleRadio(QObject *item)
{
    Q_Q(CheckGroup);
    if (!item || (item && !item->property("checked").isValid()))
        return;
    if (item->property("checked").toBool()) {
        // activation
        activeItem = item;
        activeItemId = item->property("buttonId").toInt();
        if (activeItem && activeItem->property(RADIO_ITEM).isValid()) {
            if (prevItem && (prevItem != activeItem)) {
                prevItem->setProperty("checked", QVariant(false));
            }
            prevItem = activeItem;
        }
        emit q->activeItemChanged();
    } else {
        // deactivation, reset focused property
        if (item->metaObject()->indexOfProperty("focused"))
            item->setProperty("focused", QVariant(false));
    }
}

void CheckGroupPrivate::addItem(QObject *item, int checkId)
{
    item->setProperty(RADIO_ITEM, QVariant(true));
    signalMap->removeMappings(item);
    signalMap->setMapping(item, checkId);
    QObject::connect(item, SIGNAL(checkedChanged()), signalMap, SLOT(map()));
}

void CheckGroupPrivate::delItem(QObject *item)
{
    QObject::disconnect(item, SIGNAL(checkedChanged()), signalMap, SLOT(map()));
    signalMap->removeMappings(item);
    item->setProperty(RADIO_ITEM, QVariant());
}

/*=============================================================================
==============================================================================*/
CheckGroup::CheckGroup(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    d_ptr(new CheckGroupPrivate(this))
{
    //d_ptr->_q_updateParent();
    connect(this, SIGNAL(parentChanged()), SLOT(_q_updateParent()));
}

CheckGroup::~CheckGroup()
{
}

void CheckGroup::componentComplete()
{
    Q_D(CheckGroup);
    if (d->layout) {
        foreach (QObject *c, d->layout->children()) {
            StyledItem *item = qobject_cast<StyledItem*>(c);
            if (item)
                qDebug() << "  group item:" << item;
        }
    }
    QDeclarativeItem::componentComplete();
}

void CheckGroup::addGroupItem(QObject *item)
{
    if (!item)
        return;

    Q_D(CheckGroup);
    static bool updating = false;
    QVariant pRadio = item->property("radio");
    QVariant pButtonId = item->property("buttonId");

    if (!updating && pRadio.isValid() && pButtonId.isValid()) {
        bool isRadio = pRadio.toBool();
        int id = pButtonId.toInt();

        if (isRadio && (id < 0) && d->m_autoId) {
            updating = true;
            id = ++d->lastId;
            item->setProperty("buttonId", QVariant(id));
            updating = false;
        }

        if (!isRadio || (id < 0)) {
            QVariant mapped = item->property(RADIO_ITEM);
            if (mapped.isValid() && mapped.toBool()) {
                d->delItem(item);
            }
        } else {
            d->addItem(item, id);
            d->handleRadio(item);
        }
    }
}

void CheckGroup::removeGroupItem(QObject *item)
{
    if (!item)
        return;
    Q_D(CheckGroup);
    d->delItem(item);;
}

void CheckGroup::adjustLayout()
{
    Q_D(CheckGroup);
    d->calculateItemSize();
    d->_q_updateGroupItems();
}


bool CheckGroup::autoId()
{
    Q_D(CheckGroup);
    return d->m_autoId;
}

void CheckGroup::setAutoId(bool value)
{
    Q_D(CheckGroup);
    if (d->m_autoId != value) {
        d->m_autoId = value;
        emit autoIdChanged();
    }
}

int CheckGroup::itemSize() const
{
    Q_D(const CheckGroup);
    return d->itemSize;
}
void CheckGroup::setItemSize(int size)
{
    Q_D(CheckGroup);
    if ((size != d->itemSize) && d->bodyFit) {
        d->itemSize = size;
        emit itemSizeChanged();
        if (d->itemSize)
            d->_q_updateGroupItems();
    }
}

QObject *CheckGroup::activeItem() const
{
    Q_D(const CheckGroup);
    return d->activeItem;
}

int CheckGroup::activeId() const
{
    Q_D(const CheckGroup);
    return d->activeItemId;
}
void CheckGroup::setActiveId(int id)
{
    Q_D(CheckGroup);
    if ((id >= 0) && (d->activeItemId != id)) {
        d->_q_uncheckPrev(id);
    }
}

QDeclarativeItem *CheckGroup::layout() const
{
    Q_D(const CheckGroup);
    return d->layout;
}
void CheckGroup::setLayout(QDeclarativeItem *item)
{
    Q_D(CheckGroup);
    if (d->layout != item) {
        if (d->layout) {
            disconnect(d->layout, SIGNAL(childrenChanged()), this, SLOT(_q_updateGroupItems()));
            d->layout->deleteLater();
        }
        d->layout = item;
#ifdef TRACE_GROUP
        qDebug() << "Group layout is" << d->layout;
#endif
        if (d->layout) {
            d->layout->setParentItem(this);
            connect(d->layout, SIGNAL(childrenChanged()), SLOT(_q_updateGroupItems()));
        }
        emit groupBodyChanged();
    }
}

Qt::Orientation CheckGroup::orientation() const
{
    Q_D(const CheckGroup);
    return d->orientation;
}
void CheckGroup::setOrientation(Qt::Orientation o)
{
    Q_D(CheckGroup);
    if (d->orientation != o) {
        d->orientation = o;
        d->calculateItemSize();
        d->_q_updateGroupItems();
        emit orientationChanged();
    }
}

bool CheckGroup::resizeChildrenToGroup() const
{
    Q_D(const CheckGroup);
    return d->bodyFit;
}
void CheckGroup::setResizeChildrenToGroup(bool v)
{
    Q_D(CheckGroup);
    if (d->bodyFit != v) {
        d->bodyFit = v;
        d->calculateItemSize();
        d->_q_updateGroupItems();
        emit resizeChildrenToGroupChanged();
    }
}

#include "moc_checkgroup.cpp"
