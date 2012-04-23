#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QObject>
#include <QXmlStreamReader>
#include "globaldefs.h"
#include "theme.h"

#define DEF_THEME               "default_theme"
#define VARIABLES               "Variables"

class StyleManager : public QObject
{
    Q_OBJECT
public:
    explicit StyleManager(QObject *parent = 0);
    ~StyleManager();

    static StyleManager *instance();
    static void initialize(QDeclarativeContext *context);
    // theme interface
    static void registerTheme(Theme *theme);
    static void unloadTheme(Theme *theme);
    static Theme *activeTheme();

signals:
    void themeChanged();
    
public slots:
    void activateTheme(const QString &theme);
private:

    QDeclarativeContext *rootContext;
    QHash<QString, Theme*> themeHash;
    QString m_activeTheme;
};

#endif // STYLEMANAGER_H
