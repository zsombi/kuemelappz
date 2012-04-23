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

void Theme::classBegin()
{
#ifdef TRACE_THEME
    qDebug() << "Theme component parsing starts";
#endif
}

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
            // set is in, check if type has been added...
            StyleSet set(i.value());
            Style::Set setType = item->setType();
            StyleSet::const_iterator j = set.find(setType);
            if ((j == set.end()) || (j.key() != setType)) {
                set[setType] = item;
                m_styleMap.insert(item->name(), set);
            }
        } else {
            // set has not been added yet
            StyleSet set;
            set.insert(item->setType(), item);
            m_styleMap.insert(item->name(), set);
        }
    }

    emit styleSetChanged();

}

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

void Theme::children_append(QDeclarativeListProperty<Style> *list, Style *item)
{
    Theme *theme = static_cast<Theme*>(list->object);
#ifdef TRACE_THEME
    qDebug() << "Theme style::" << item->name() << "/" << item->setType();
#endif
    theme->m_styleSet << item;
}
int Theme::children_count(QDeclarativeListProperty<Style> *list)
{

}
Style *Theme::children_at(QDeclarativeListProperty<Style> *list, int index)
{
    return 0;
}
void Theme::children_clear(QDeclarativeListProperty<Style> *list)
{

}

QDeclarativeListProperty<Style> Theme::styleSet()
{
    //return QDeclarativeListProperty<Style>(this, &m_styleMap, children_append, 0, 0, 0);
    return QDeclarativeListProperty<Style>(this, m_styleSet);
}

Style *Theme::style(const QString &name, Style::Set set)
{
#ifdef TRACE_THEME
    qDebug() << "Locate style" << name << "/" << set;
#endif
    // some components using styles get initialized before theme is completed...
    if (m_styleMap.isEmpty())
        componentComplete();
    ThemeSet::const_iterator i = m_styleMap.find(name);
    if ((i != m_styleMap.end()) && (i.key() == name)) {
        StyleSet setMap(i.value());
        StyleSet::const_iterator j = setMap.find(set);
        if ((j != setMap.end()) && (j.key() == set)) {
#ifdef TRACE_THEME
            qDebug() << "Found" << j.value();
#endif
            return j.value();
        }
    }
    return 0;
}
