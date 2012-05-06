/**
  StyledItem - base item for all WidgetSet components that support theming.
  The style is chosen based on the style name and type given. Styles can be defined
  either by themes or can be local. Upon theme change, themed components get notified
  about the change and the layout is refreshed automatically because of property binding.

  Properties:
    property styleName: string
        Style identifier string i.e. "Button" or "ToolButton"

    property styleType: StyleType
        Current type of the style. The component's layout can be changed just by altering
        this property, assuming that the type set has been declared either locally or in
        the active theme. Upon type change the onStyleChanged signal is emited!

    property style: Style read-only
        Represents the currently active style set object, either local or theme containing one.

    property localStyles: list<Style>
        List of locally declared styles. The list should contain only control specific styles,
        and therefore the amount should be limited to the types needed by the control. Styles
        declared locally must also have names set and the StyledItem should use the name
        defined for the styles.

    property controlListItem: Item
        This item is set by the components that drive several sub-components key handling
        (i.e. tab-order).

    TODO: method for styles available?

  */
#include "styleditem.h"
#include "styleditem_p.h"
#include "stylemanager.h"
#include "checkgroup.h"
#include "style.h"
#include "theme.h"

#include <QtDeclarative/qdeclarative.h>
#include <QKeyEvent>

//#define TRACE_STYLEDITEM

StyledItemPrivate::StyledItemPrivate(StyledItem *qq) :
    q_ptr(qq),
    styleName(""),
    currentStyle(Style::LastStyleType),
    controlListItem(0)
{
    // connect enableChanged() to handle Dimmed style type
    QObject::connect(q_ptr, SIGNAL(enabledChanged()), q_ptr, SLOT(_q_activateStyle()));
    // connect parentChanged() to add component to CheckGroup
    QObject::connect(q_ptr, SIGNAL(parentChanged()), q_ptr, SLOT(_q_updateParent()));
    // this may not be needed after all...
    QObject::connect(q_ptr, SIGNAL(focusChanged(bool)), q_ptr, SLOT(_q_handleFocusChange(bool)));
    // connect to theme manager to activate theme and style changes
    QObject::connect(StyleManager::instance(), SIGNAL(themeChanged()), q_ptr, SLOT(_q_activateStyle()));
}

StyledItemPrivate::~StyledItemPrivate()
{
    // for some reason disconnects are not happening automatically on Linux\Windows platforms ?!
    if (controlListItem)
        QObject::disconnect(controlListItem, SIGNAL(childrenChanged()), q_ptr, SLOT(_q_updateKeyOrder()));
    QObject::disconnect(StyleManager::instance(), SIGNAL(themeChanged()), q_ptr, SLOT(_q_activateStyle()));
    QObject::disconnect(q_ptr, SIGNAL(enabledChanged()), q_ptr, SLOT(_q_activateStyle()));
    QObject::disconnect(q_ptr, SIGNAL(parentChanged()), q_ptr, SLOT(_q_updateParent()));
    QObject::disconnect(q_ptr, SIGNAL(focusChanged(bool)), q_ptr, SLOT(_q_handleFocusChange(bool)));
}

/*
  lookup for style of a specific type, first in local styles then in theme
*/
Style* StyledItemPrivate::styleForType(Style::StyleType type) const
{
    // 1: check local styles first
    QListIterator<Style*> pl(localStyles);
    while (pl.hasNext()) {
        Style *item = pl.next();
        if ((item->name() == styleName) && (item->type() == type))
            return item;
    }
    // if none found check for in theme styles
    return (StyleManager::instance()) ? StyleManager::instance()->activeTheme()->style(styleName, type) : 0;
}

/*
  Private slot called either
    - when Theme manager changes the active theme, so styled items can update
        the layout with the style values
    - when style type gets changed

  The method also makes corrections on the style type as the theme/local set may
  not have the style type declared. In these cases the appropriate style type is
  chosen.
*/
void StyledItemPrivate::_q_activateStyle()
{
    Q_Q(StyledItem);
    if (!StyleManager::activeTheme())
        currentStyle = Style::LastStyleType;
    else {
        if (!q->isEnabled())
            currentStyle = Style::Dimmed;
        // todo: cache this!
        else if (!styleForType(currentStyle))
            currentStyle = Style::Normal;
    }
    emit q->styleChanged();
}

/*
  Handle parent change to add component to CheckGroup
*/
void StyledItemPrivate::_q_updateParent()
{
    Q_Q(StyledItem);
    QDeclarativeItem *parent = q->parentItem();
    if (!parent || (parent && !parent->parentItem()))
        return;
    // dirrect parent can be either CheckGroup or a positioner element
    CheckGroup *grp = qobject_cast<CheckGroup*>(parent);
    if (!grp)
        grp = qobject_cast<CheckGroup*>(parent->parentItem());
    // add item to group, group will check the rest
    if (grp)
        grp->addGroupItem(q);
}

void StyledItemPrivate::_q_handleFocusChange(bool f)
{
#ifdef TRACE_STYLEDITEM
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
    //setFlag(ItemIsFocusScope);
    //setFlag(ItemIsSelectable);
}

StyledItem::~StyledItem()
{
}

void StyledItem::keyPressEvent(QKeyEvent *event)
{
    // TODO: at some point...
    QDeclarativeItem::keyPressEvent(event);
    if (event) {
#ifdef TRACE_STYLEDITEM
        qDebug()<< this <<"key pressed:" << event->key();
#endif
    }
}

/*******************************************************************************
    PROPERTY GETTER/SETTER
*******************************************************************************/
Style *StyledItem::style() const
{
    Q_D(const StyledItem);
    Style *ret = d->styleForType(d->currentStyle);
#ifdef TRACE_STYLEDITEM
    if (!ret)
        qDebug() << "Style: " <<ret << "for type" << d->currentStyle;
    else
        qDebug() << "Style:" << ret << "(" << ret->name() << "/" << ret->type() <<")" << d->currentStyle;
#endif
    return ret;
}

Style::StyleType StyledItem::styleType() const
{
    Q_D(const StyledItem);
    return d->currentStyle;
}
void StyledItem::setStyleType(Style::StyleType type)
{
    Q_D(StyledItem);
    if (d->currentStyle != type) {
        d->currentStyle = type;
#ifdef TRACE_STYLEDITEM
        qDebug() << "currentStyle:" << "(" << d->styleName << "/" << d->currentStyle <<")";
#endif
        // check if there is such kind of style in the theme set;
        // if not, apply the closest one
        d->_q_activateStyle();
    }
}

QDeclarativeListProperty<Style> StyledItem::localStyles()
{
    Q_D(StyledItem);
    return QDeclarativeListProperty<Style>(this, d->localStyles);
}

// styleName proeprty getter/setter
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
        // reset style type to Normal and get the style object
        d->currentStyle = Style::Normal;
        d->_q_activateStyle();

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
#ifdef TRACE_STYLEDITEM
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

