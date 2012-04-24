#include "stylemanager.h"
#include <QMultiHash>
#include <QResource>
#include <QFile>
#include <QDeclarativeContext>
#include <QDeclarativePropertyMap>
#include <QDebug>
#include <QCoreApplication>

//#define TRACE_THEMES

Q_GLOBAL_STATIC(StyleManager, styleManager)

/*
  ---------------------PUBLIC---------------------
  */

StyleManager::StyleManager(QObject *parent) :
    QObject(parent)
{
}

StyleManager::~StyleManager()
{
#ifdef TRACE_THEMES
    qDebug() << "Deleting StyleManager";
#endif
}

StyleManager *StyleManager::instance()
{
    return styleManager();
}

void StyleManager::initialize(QDeclarativeContext *context)
{
    StyleManager *sman = styleManager();
    sman->rootContext = context;
    context->setContextProperty("themeManager", sman);
}

void StyleManager::registerTheme(Theme *theme)
{
    StyleManager *sman = styleManager();
    sman->themeHash.insert(theme->name(), theme);
    if (sman->m_activeTheme.isEmpty())
        sman->activateTheme(theme->name());
}

void StyleManager::unloadTheme(Theme *theme)
{
    // todo:
    QString res(theme->resource());
    if (!res.isEmpty() && (res != DEF_THEME)) {
        QResource::unregisterResource(res, "/theme");
    }
    styleManager()->activateTheme("default");
}

Theme *StyleManager::activeTheme()
{
    StyleManager *sman = styleManager();
    QHash<QString, Theme*>::const_iterator i = sman->themeHash.find(sman->m_activeTheme);
    if ((i != sman->themeHash.end()) && (i.key() == sman->m_activeTheme))
        return i.value();
    return 0;
}

//**********************************************************************************
void StyleManager::activateTheme(const QString &theme)
{
    Theme *active = activeTheme();
    if (active)
        unloadTheme(active);

    m_activeTheme = theme;
    // set context variable "Theme" with the curent one
    active = activeTheme();
    // load theme resources
    QString res(active->resource());
    if (!res.isEmpty() && (res != DEF_THEME))
        QResource::registerResource(res, ":/theme");

    rootContext->setContextProperty("THEME", active);
    emit themeChanged();
}

