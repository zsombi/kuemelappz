/**
  StyleManager - internal class managing theme registration and selection.
  Its functionality can be accessed from QML through <b>themeManager</b> variable,
  but the active theme is published through the <b>THEME</b> variable. Upon theme
  activation the theme resources are loaded and mapped to ":/theme".

  Signals:
    themeChanged()
        The signal is triggered when the theme is changed and loaded. StyledItem
        elements are connecting to this signal to update style.

  Slots:
    activateTheme(string theme)
        The slot activates a registered theme. Themes activated through this slot
        must be included in the application, preferably into the root component.

*/
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

