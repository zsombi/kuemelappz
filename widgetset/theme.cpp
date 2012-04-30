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

    default property styleSet: list<Style> read-only
        List of Style elements defining the styleset. The list is also hashed to ease
        searching in the styleset. The hashing happens upon theme component completion
        or at first access, in case some WidgetSet component does an early access to
        the theme.
  */

#include "theme.h"
#include "globaldefs.h"
#include "stylemanager.h"

//#define TRACE_THEME

Theme::Theme(QObject *parent) :
    QObject(parent)
{
}

Theme::~Theme()
{
#ifdef TRACE_THEME
    qDebug() << "Theme" << m_name << "unloaded";
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
#ifdef TRACE_THEME
    qDebug() << "Theme component parsing ends";
#endif
    // parse theme list and classify in styleSets
    QListIterator<Style*> pl(m_styleSet);
    while (pl.hasNext()) {
        Style *item = pl.next();
#ifdef TRACE_THEME
        qDebug() << "Hashing style::" << item->name() << "/" << item->setType();
#endif

        ThemeSet::const_iterator i = m_styleMap.find(item->name());
        if ((i != m_styleMap.end()) && (i.key() == item->name())) {
            // style is in, check if type has been added...
            StyleSet set(i.value());
            Style::StyleType setType = item->type();
            StyleSet::const_iterator j = set.find(setType);
            if ((j == set.end()) || (j.key() != setType)) {
                set[setType] = item;
                m_styleMap.insert(item->name(), set);
            }
        } else {
            // set has not been added yet
            StyleSet set;
            set.insert(item->type(), item);
            m_styleMap.insert(item->name(), set);
        }
    }

    emit styleSetChanged();
}

// name property getter/setter
QString Theme::name() const
{
    return m_name;
}
void Theme::setName(const QString &s)
{
    if (m_name != s)  {
        m_name = s;
        // register theme
        StyleManager::registerTheme(this);
        emit nameChanged();
    }
}

// resource property getter/setter
QString Theme::resource() const
{
    return m_resource;
}
void Theme::setResource(const QString &s)
{
    if (m_resource != s) {
        // todo: check if theme is active, then unregister resources
        m_resource = s;
        emit resourceChanged();
    }
}

// styleset property getter
QDeclarativeListProperty<Style> Theme::styleSet()
{
    return QDeclarativeListProperty<Style>(this, m_styleSet);
}

/**
  Method called by StyleManager to look after a given style and type.

  param name: style name
  param type: style type (see Style for more)

  returns: Style object or 0 if not found
  */
Style *Theme::style(const QString &name, Style::StyleType type)
{
#ifdef TRACE_THEME
    qDebug() << "Locate style" << name << "/" << type;
#endif
    // some components using styles get initialized before theme is completed...
    if (m_styleMap.isEmpty())
        componentComplete();
    ThemeSet::const_iterator i = m_styleMap.find(name);
    if ((i != m_styleMap.end()) && (i.key() == name)) {
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
