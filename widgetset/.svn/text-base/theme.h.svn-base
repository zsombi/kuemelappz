#ifndef THEME_H
#define THEME_H

#include <QObject>
#include "style.h"
#include <qdeclarative.h>
#include <QDeclarativeParserStatus>

typedef QHash<Style::Set, Style*> StyleSet;
typedef QHash<QString, StyleSet> ThemeSet;

class Theme : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString resource READ resource WRITE setResource NOTIFY resourceChanged)
    Q_PROPERTY(QDeclarativeListProperty<Style> styleSet READ styleSet NOTIFY styleSetChanged DESIGNABLE false)
    Q_CLASSINFO("DefaultProperty", "styleSet")
public:
    Theme(QObject *parent = 0);
    ~Theme();

    // from QDeclarativeParserStatus
    void classBegin();
    void componentComplete();
    
    QString name() const;
    void setName(const QString &s);
    QString resource() const;
    void setResource(const QString &s);
    QDeclarativeListProperty<Style> styleSet();

    Style *style(const QString &name, Style::Set set);
signals:

    void nameChanged();
    void resourceChanged();
    void styleSetChanged();
    
public slots:

private:
    QList<Style*> m_styleSet;
    ThemeSet m_styleMap;
    QString m_name;
    QString m_resource;

    static void children_append(QDeclarativeListProperty<Style> *list, Style *item);
    static int children_count(QDeclarativeListProperty<Style> *list);
    static Style *children_at(QDeclarativeListProperty<Style> *list, int index);
    static void children_clear(QDeclarativeListProperty<Style> *list);
};

QML_DECLARE_TYPE(Theme)

#endif // THEME_H
