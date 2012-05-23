/**
  StyledItem - base item for all WidgetSet components that support theming.
  The style is chosen based on the style name and type given. Styles can be defined
  either by themes or can be local. Upon theme change, themed components get notified
  about the change and the layout is refreshed automatically because of property binding.

  Another fuctionality of the item is the automatic tab-order handling. The tab-order is
  considered to be the order of the children, the focus handling is driven inside the
  StyledItem.

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

    property focusType: FocusType
        Specifies the behavior of the styled item.
        StyledItem.Decorative - purely decorative item
        StyledItem.Focusable - the item captures key events
        StyledItem.FocusGroup - the item groups focusable elements

    property controlId: int
        Control identifier

  Slots:
    focusOnNext()
    focusOnPrevious()
    focusOnFirst()
    focusOnLast()

  Signals:
    activated()
    deactivated()
    invoked()

    TODO: method for styles available?

  */
#include "styleditem.h"
#include "styleditem_p.h"
#include "stylemanager.h"
#include "checkgroup.h"
#include "style.h"
#include "theme.h"
#include "widgetset.h"

#include <QtDeclarative/qdeclarative.h>
#include <QKeyEvent>
#include <QPainter>

//#define TRACE_STYLEDITEM

/*====================================================================================
  ====================================================================================*/

/*====================================================================================
  ====================================================================================*/
StyledItemPrivate::StyledItemPrivate(StyledItem *qq) :
    q_ptr(qq),
    styleName(""),
    currentStyle(Style::Undefined)
{
    // connect enableChanged() to handle Dimmed style type
    QObject::connect(q_ptr, SIGNAL(enabledChanged()), q_ptr, SLOT(_q_activateStyle()));
    // connect to theme manager to activate theme and style changes
    QObject::connect(StyleManager::instance(), SIGNAL(themeChanged()), q_ptr, SLOT(_q_activateStyle()));
    // capture child item changes
    //QObject::connect(q_ptr, SIGNAL(childrenChanged()), q_ptr, SLOT(_q_updateKeyOrder()));
}

StyledItemPrivate::~StyledItemPrivate()
{
    // for some reason disconnects are not happening automatically on Linux\Windows platforms ?!
    //QObject::disconnect(q_ptr, SIGNAL(childrenChanged()), q_ptr, SLOT(_q_updateKeyOrder()));
    QObject::disconnect(StyleManager::instance(), SIGNAL(themeChanged()), q_ptr, SLOT(_q_activateStyle()));
    QObject::disconnect(q_ptr, SIGNAL(enabledChanged()), q_ptr, SLOT(_q_activateStyle()));
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
    // if none found look after it in theme styles
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
        currentStyle = Style::Undefined;
    else {
        if (!q->isEnabled())
            currentStyle = Style::Dimmed;
        // todo: cache this!
        else if (!styleForType(currentStyle))
            currentStyle = Style::Normal;
    }
    emit q->styleChanged();
}

void StyledItemPrivate::_q_updateKeyOrder()
{
    // todo: dive deeper in children in case we have layout elements
    // consider styled items only!
#ifdef TRACE_FOCUSING
    if (q_ptr->objectName().isEmpty())
        qDebug() << __FUNCTION__ << q_ptr << "children changed";
    else
        qDebug() << __FUNCTION__ << q_ptr->objectName() << "children changed";
#endif
}




/*
  ---------------PUBLIC----------------
  */
StyledItem::StyledItem(QDeclarativeItem *parent):
    FocusControl(parent),
    d_ptr(new StyledItemPrivate(this))
{

    setImplicitWidth(100);
    setImplicitHeight(80);
}

StyledItem::~StyledItem()
{
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

#include "moc_styleditem.cpp"

