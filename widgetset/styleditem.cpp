#include "styleditem.h"
#include "styleditem_p.h"
#include "stylemanager.h"
#include "checkgroup.h"
#include "style.h"
#include "theme.h"

#include <QtDeclarative/qdeclarative.h>
#include <QKeyEvent>

//#define TRACE_THEMEITEM

StyledItemPrivate::StyledItemPrivate(StyledItem *qq) :
    q_ptr(qq),
    activeTheme(StyleManager::activeTheme()),
    styleName(""),
    currentStyle(Style::LastStyleType),
    controlListItem(0)
{
    QObject::connect(q_ptr, SIGNAL(enabledChanged()), q_ptr, SLOT(_q_activateStyle()));
    QObject::connect(q_ptr, SIGNAL(parentChanged()), q_ptr, SLOT(_q_updateParent()));
    QObject::connect(q_ptr, SIGNAL(focusChanged(bool)), q_ptr, SLOT(_q_handleFocusChange(bool)));
    // connect to theme manager to activate theme and style changes
    QObject::connect(StyleManager::instance(), SIGNAL(themeChanged()), q_ptr, SLOT(_q_activateTheme()));
}

StyledItemPrivate::~StyledItemPrivate()
{
    // for some reason disconnects are not happening in Linux\Windows platforms ?!
    if (controlListItem)
        QObject::disconnect(controlListItem, SIGNAL(childrenChanged()), q_ptr, SLOT(_q_updateKeyOrder()));
    QObject::disconnect(StyleManager::instance(), SIGNAL(themeChanged()), q_ptr, SLOT(_q_activateTheme()));
    QObject::disconnect(q_ptr, SIGNAL(enabledChanged()), q_ptr, SLOT(_q_activateStyle()));
    QObject::disconnect(q_ptr, SIGNAL(parentChanged()), q_ptr, SLOT(_q_updateParent()));
    QObject::disconnect(q_ptr, SIGNAL(focusChanged(bool)), q_ptr, SLOT(_q_handleFocusChange(bool)));
}

void StyledItemPrivate::reloadStyle()
{
    // try to load the normal one and then check whether alteration is needed
    currentStyle = Style::Normal;
    _q_activateStyle();
}

Style* StyledItemPrivate::styleForSet(Style::Set set) const
{
    // 1: check local styles first
    QListIterator<Style*> pl(localStyles);
    while (pl.hasNext()) {
        Style *item = pl.next();
        if ((item->name() == styleName) && (item->setType() == set))
            return item;
    }
    // if none found check for in theme styles
    return (activeTheme) ? activeTheme->style(styleName, set) : 0;
}

void StyledItemPrivate::_q_activateTheme()
{
    activeTheme = StyleManager::activeTheme();
    _q_activateStyle();
}

void StyledItemPrivate::_q_activateStyle()
{
    Q_Q(StyledItem);
    if (!activeTheme)
        currentStyle = Style::LastStyleType;
    else {
        if (!q->isEnabled())
            currentStyle = Style::Dimmed;
        // todo: cache this!
        else if (!styleForSet(currentStyle))
            currentStyle = Style::Normal;
    }
    emit q->styleChanged();
}

void StyledItemPrivate::_q_updateParent()
{
    // capture parent update to add control to check-group
    Q_Q(StyledItem);
    QDeclarativeItem *parent = q->parentItem();
    if (!parent || (parent && !parent->parentItem()))
        return;
    CheckGroup *grp = qobject_cast<CheckGroup*>(parent);
    if (!grp)
        grp = qobject_cast<CheckGroup*>(parent->parentItem());
    // item is not grouped?
    if (!grp)
        return;
    grp->updateGroupItem(q);
}

void StyledItemPrivate::_q_handleFocusChange(bool f)
{
#ifdef TRACE_THEMEITEM
    qDebug() << __FUNCTION__ << q_ptr << "focus =" << f;
#endif
}

void StyledItemPrivate::_q_updateKeyOrder()
{
    if (controlListItem) {
        QDeclarativeListReference ref(controlListItem, "children");
        // todo: dive deeper in children in case we have layout elements
        // consider styled items only!
    }
}

/*
  ---------------PUBLIC----------------
  */
StyledItem::StyledItem(QDeclarativeItem *parent):
    QDeclarativeItem(parent),
    d_ptr(new StyledItemPrivate(this))
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:
    
    // setFlag(ItemHasNoContents, false);
    setImplicitWidth(100);
    setImplicitHeight(80);
    //setFlag(ItemIsFocusable);
    //setFlag(ItemIsSelectable);
}

StyledItem::~StyledItem()
{
}

void StyledItem::keyPressEvent(QKeyEvent *event)
{
    QDeclarativeItem::keyPressEvent(event);
    if (event) {
#ifdef TRACE_THEMEITEM
        qDebug()<< this <<"key pressed:" << event->key();
#endif
    }
}

Style *StyledItem::style() const
{
    Q_D(const StyledItem);
    Style *ret = d->styleForSet(d->currentStyle);
#ifdef TRACE_THEMEITEM
    qDebug() << "Style:" << ret << "(" << ret->name() << "/" << ret->setType() <<")" << d->currentStyle;
#endif
    return ret;
}
Style::Set StyledItem::currentStyleSet() const
{
    Q_D(const StyledItem);
    return d->currentStyle;
}
void StyledItem::setCurrentStyleSet(Style::Set set)
{
    Q_D(StyledItem);
    if (d->currentStyle != set) {
        d->currentStyle = set;
#ifdef TRACE_THEMEITEM
        qDebug() << "currentStyle:" << "(" << d->control << "/" << d->currentStyle <<")";
#endif
        // todo: check if there is such kind of style in the theme set;
        // if not, apply the closest one
        d->_q_activateStyle();
    }
}

QDeclarativeListProperty<Style> StyledItem::localStyles()
{
    Q_D(StyledItem);
#ifdef TRACE_THEMEITEM
    qDebug() << "LOCAL STYLES!!";
#endif
    return QDeclarativeListProperty<Style>(this, d->localStyles);
}

QString StyledItem::styleName() const
{
    Q_D(const StyledItem);
    return d->styleName;
}
void StyledItem::setStyleName(const QString &val)
{
    Q_D(StyledItem);
    if (d->styleName != val) {
        d->styleName = val;
        d->reloadStyle();
        emit styleNameChanged();
    }
}
QDeclarativeItem* StyledItem::controlListItem() const
{
    Q_D(const StyledItem);
    return d->controlListItem;
}
void StyledItem::setControlListItem(QDeclarativeItem *list)
{
#ifdef TRACE_THEMEITEM
    qDebug() << __FUNCTION__ << list;
#endif
    Q_D(StyledItem);
    if (d->controlListItem != list) {
        if (d->controlListItem)
            QObject::disconnect(d->controlListItem, SIGNAL(childrenChanged()), this, SLOT(_q_updateKeyOrder()));
        d->controlListItem = list;
        if (d->controlListItem) {
            QObject::connect(d->controlListItem, SIGNAL(childrenChanged()), this, SLOT(_q_updateKeyOrder()));
            // do the first update as children may already been set by this time
            d->_q_updateKeyOrder();
        }
        emit controlListItemChanged();
    }
}

#include "moc_styleditem.cpp"

