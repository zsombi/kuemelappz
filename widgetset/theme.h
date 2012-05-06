#ifndef THEME_H
#define THEME_H

#include <QObject>
#include "style.h"
#include <qdeclarative.h>
#include <QDeclarativeParserStatus>
#include <QDeclarativeItem>

typedef QHash<Style::StyleType, Style*> StyleSet;
typedef QHash<QString, StyleSet> ThemeSet;

class Theme : public QDeclarativeItem
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString resource READ resource WRITE setResource NOTIFY resourceChanged)
public:
    Theme(QDeclarativeItem *parent = 0);
    ~Theme();

    // from QDeclarativeParserStatus
    void classBegin();
    void componentComplete();
    
    QString name() const;
    void setName(const QString &s);
    QString resource() const;
    void setResource(const QString &s);

    Style *style(const QString &name, Style::StyleType type);
signals:

    void nameChanged();
    void resourceChanged();
    void styleSetChanged();
    
public slots:

private:
    ThemeSet m_styleMap;
    QString m_name;
    QString m_resource;
};

QML_DECLARE_TYPE(Theme)

#endif // THEME_H
