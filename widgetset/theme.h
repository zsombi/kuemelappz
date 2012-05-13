#ifndef THEME_H
#define THEME_H

#include <QObject>
#include "style.h"
#include <qdeclarative.h>
#include <QDeclarativeParserStatus>
#include <QDeclarativeItem>

typedef QHash<Style::StyleType, Style*> StyleSet;
typedef QHash<QString, StyleSet> ThemeSet;

class ThemePrivate;
class Theme : public QDeclarativeItem
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString resource READ resource WRITE setResource NOTIFY resourceChanged)
    Q_PROPERTY(Style* sizes READ sizes NOTIFY sizesChanged)
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
    Style *sizes() const;

    Q_INVOKABLE Style *style(const QString &name, Style::StyleType type = Style::Normal);
signals:

    void nameChanged();
    void resourceChanged();
    void styleSetChanged();
    void sizesChanged();
    
public slots:

private:
    Q_DISABLE_COPY(Theme)
    Q_DECLARE_PRIVATE(Theme)
    QScopedPointer<ThemePrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_updateMeasurementObject())
};

QML_DECLARE_TYPE(Theme)

#endif // THEME_H
