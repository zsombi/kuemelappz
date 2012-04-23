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
    bodyType(CheckGroup::None),
    activeItemId(-1),
    m_autoId(false),
    itemSize(0), itemSpacing(0),
    bodyFit(CheckGroup::Default),
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
#ifdef TRACE_GROUP
    qDebug() << __FUNCTION__;
#endif
    if (!layout)
        return;
    calculateBodySize();
    QList<QGraphicsItem*> children = layout->childItems();
    int childCount = children.length();
    int i = 0, offs = 0;
    foreach (QGraphicsItem *child, children) {
        StyledItem *uiItem = qobject_cast<StyledItem*>(child);
#ifdef TRACE_GROUP
        qDebug() << "index:" << i++ << ", count:" << childCount
                 << uiItem;
#endif
        if (uiItem) {
            i++;
            if (itemSize > 0) {
                switch (bodyType) {
                case CheckGroup::Horizontal:
                    QDeclarativeProperty::write(uiItem, "width", itemSize);
                    QDeclarativeProperty::write(uiItem, "x", offs);
                    break;
                case CheckGroup::Vertical:
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

void CheckGroupPrivate::calculateBodySize()
{
    if (bodyFit != CheckGroup::FitToGroup)
        return;
    if (!layout)
        return;

    // Row, Column or Flow are all enumerated as child items, also thers may ->
    // these need to be excluded
    int items = 0;
    foreach (QGraphicsItem *child, layout->childItems()) {
        if (qobject_cast<StyledItem*>(child))
            items++;
    }
    if (!items)
        return;

    Q_Q(CheckGroup);
    switch (bodyType) {
    case CheckGroup::Horizontal:
        itemSize = q->width() / items;
        break;
    case CheckGroup::Vertical:
        itemSize = q->height() / items;
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
#ifdef TRACE_GROUP
        qDebug() << "active:: " << item;
#endif
        // activation
        activeItem = item;
        activeItemId = item->property("buttonId").toInt();
#ifdef TRACE_GROUP
        qDebug() << "Activate control ID" << activeItemId;
#endif
        if (activeItem && activeItem->property(RADIO_ITEM).isValid()) {
            if (prevItem && (prevItem != activeItem)) {
#ifdef TRACE_GROUP
                qDebug() << "START Uncheck prev::" << prevItem;
#endif
                prevItem->setProperty("checked", QVariant(false));
#ifdef TRACE_GROUP
                qDebug() << "END Uncheck prev:: " << prevItem;
#endif
            }
#ifdef TRACE_GROUP
            else
                qDebug() << "NO PREV ACTIVE ITEM";
#endif
            prevItem = activeItem;
        }
        emit q->activeItemChanged();
    } else {
        // deactivation, reset focused property
#ifdef TRACE_GROUP
        qDebug() << "deactivate " << item;
#endif
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

void CheckGroup::updateGroupItem(QObject *item)
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

void CheckGroup::adjustLayout()
{
    Q_D(CheckGroup);
    d->calculateBodySize();
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
    if ((size != d->itemSize) && (d->bodyFit != FitToGroup)) {
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

CheckGroup::BodyType CheckGroup::bodyType() const
{
    Q_D(const CheckGroup);
    return d->bodyType;
}
void CheckGroup::setBodyType(BodyType type)
{
    Q_D(CheckGroup);
    if (d->bodyType != type) {
        d->bodyType = type;
        d->calculateBodySize();
        d->_q_updateGroupItems();
        emit bodyTypeChanged();
    }
}

CheckGroup::BodyFit CheckGroup::bodyFit() const
{
    Q_D(const CheckGroup);
    return d->bodyFit;
}
void CheckGroup::setBodyFit(BodyFit type)
{
    Q_D(CheckGroup);
    if (d->bodyFit != type) {
        d->bodyFit = type;
        d->calculateBodySize();
        d->_q_updateGroupItems();
        emit bodyFitChanged();
    }
}

#include "moc_checkgroup.cpp"
