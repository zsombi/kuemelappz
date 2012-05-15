/**
    Theme - non-visual element that holds style elements defining the look-and-feel
    for WidgetSet components. A complete theme (or styleset) should be decrated in
    a separate component, listing additional properties the component set requires.

    For reference and example see DefaultTheme.qml.

    An application can hold several theme definitions. The application designer can
    reuse the DefaultTheme and change the name and resource properties. Themes are
    registered in native code by the StyleManager (see more there). StyledItems can
    get "styled" by defining a valid style name that coresponds to the styled item
    (e.g. Button component should use styles declared using ButtonStyle style element).

    Properties:

    proeprty name: string
        Defines and identifies the styleset (theme).

    property resource: string
        File name with full path to the binary resource file defining the resources
        used by the styleset. The resource file will be loaded once the theme is
        activated, and all th eresources are mounter to the "/theme" prefix, therefore
        styles accessing the resources should be aware of this. default resources
        are prefixed with "/default".
  */

#include "theme.h"
#include "globaldefs.h"
#include "stylemanager.h"
#include "widgetset.h"
#include "applicationitem.h"

//#define TRACE_THEME

class ThemePrivate {
    Q_DECLARE_PUBLIC(Theme)
public:
    ThemePrivate(Theme *qq);
    ~ThemePrivate(){}

    void _q_updateMeasurementObject();

    Theme *q_ptr;
    ThemeSet m_styleMap;
    Style *sizesHolder;
    Screen::Orientation lastOrientation;
    QString m_name;
    QString m_resource;
};

ThemePrivate::ThemePrivate(Theme *qq) :
    q_ptr(qq),
    sizesHolder(0),
    lastOrientation(Screen::Automatic)
{
    // capture orientation changes to chose the proper measurements
    QObject::connect(Screen::instance(), SIGNAL(orientationChanged()), q_ptr, SLOT(_q_updateMeasurementObject()));
}

void ThemePrivate::_q_updateMeasurementObject()
{
    Q_Q(Theme);
    Screen *screen = Screen::instance();
    if (!sizesHolder || (sizesHolder && (lastOrientation != screen->orientation()))) {
        QString sorientation = screen->orientationString();
        // remove "Inverse" from orientation, for measurements it does not matter
        sorientation.remove("Inverse");
        // 1st priority: type/density/orientation
        QString sizesPath = QString("%1/%2/%3").
                arg(screen->typeString()).
                arg(screen->densityString()).
                arg(sorientation);
        sizesHolder = q->style(sizesPath);
        if (!sizesHolder) {
            // 2nd priority: density/orientation
            sizesPath = QString("%1/%2").
                    arg(screen->densityString()).
                    arg(sorientation);
            sizesHolder = q->style(sizesPath);
        }
        if (!sizesHolder) {
            // 3rd properity: orientation
            sizesHolder = q->style(sorientation);
        }
#ifdef TRACE_THEME
        qDebug() << "Theme measurement: "<<sizesHolder << "for path:" << sizesPath;
#endif
        emit q->sizesChanged();
    }
}

/***********************************************************************************
***********************************************************************************/

/**
  Inherited from QDeclarativeItem to be able to add items (i.e. Text) to declare
  font sizes and resolution based layout items.
  */
Theme::Theme(QDeclarativeItem *parent) :
    //QObject(parent)
    QDeclarativeItem(parent),
    d_ptr(new ThemePrivate(this))
{
}

Theme::~Theme()
{
#ifdef TRACE_THEME
    qDebug() << "Theme" << d_ptr->m_name << "unloaded";
#endif
}

// QDeclarativeParserStatus methods
void Theme::classBegin()
{
}

// Hash styleset for fast search
// some components do access the styleset before this component gets completed...?
void Theme::componentComplete()
{
    Q_D(Theme);
#ifdef TRACE_THEME
    qDebug() << "Theme component parsing ends";
#endif
    // parse theme list and classify in styleSets
    //QListIterator<Style*> pl(m_styleSet);
    //while (pl.hasNext()) {
        //Style *item = pl.next();
    QList<Style*> cl = findChildren<Style*>();
    foreach(Style *item, cl) {
#ifdef TRACE_THEME
        qDebug() << "Hashing style::" << item->name() << "/" << item->type();
#endif

        ThemeSet::const_iterator i = d->m_styleMap.find(item->name());
        if ((i != d->m_styleMap.end()) && (i.key() == item->name())) {
            // style is in, check if type has been added...
            StyleSet set(i.value());
            Style::StyleType setType = item->type();
            StyleSet::const_iterator j = set.find(setType);
            if ((j == set.end()) || (j.key() != setType)) {
                set[setType] = item;
                d->m_styleMap.insert(item->name(), set);
            }
        } else {
            // set has not been added yet
            StyleSet set;
            set.insert(item->type(), item);
            d->m_styleMap.insert(item->name(), set);
        }
    }

    // update measurement element
    d->_q_updateMeasurementObject();
    emit styleSetChanged();
}

// name property getter/setter
QString Theme::name() const
{
    Q_D(const Theme);
    return d->m_name;
}
void Theme::setName(const QString &s)
{
    Q_D(Theme);
    if (d->m_name != s)  {
        d->m_name = s;
        // register theme
        StyleManager::registerTheme(this);
        emit nameChanged();
    }
}

// resource property getter/setter
QString Theme::resource() const
{
    Q_D(const Theme);
    return d->m_resource;
}
void Theme::setResource(const QString &s)
{
    Q_D(Theme);
    if (d->m_resource != s) {
        // todo: check if theme is active, then unregister resources
        d->m_resource = s;
        emit resourceChanged();
    }
}
Style *Theme::sizes() const
{
    Q_D(const Theme);
    return d->sizesHolder;
}

/**
  Method called by StyleManager to look after a given style and type.

  param name: style name
  param type: style type (see Style for more)

  returns: Style object or 0 if not found
  */
Style *Theme::style(const QString &name, Style::StyleType type)
{
    Q_D(Theme);
#ifdef TRACE_THEME
    qDebug() << "Locate style" << name << "/" << type;
#endif
    // some components using styles get initialized before theme is completed...
    if (d->m_styleMap.isEmpty())
        componentComplete();
    ThemeSet::const_iterator i = d->m_styleMap.find(name);
    if ((i != d->m_styleMap.end()) && (i.key() == name)) {
        StyleSet setMap(i.value());
        StyleSet::const_iterator j = setMap.find(type);
        if ((j != setMap.end()) && (j.key() == type)) {
#ifdef TRACE_THEME
            qDebug() << "Found" << j.value();
#endif
            return j.value();
        }
    }
    return 0;
}

#include "moc_theme.cpp"
