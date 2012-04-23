#ifndef STYLE_H
#define STYLE_H

#include <QObject>
#include <qdeclarative.h>

class Style : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Set set READ setType WRITE setSetType NOTIFY setChanged)

    Q_ENUMS(Set)
public:
    enum Set {
        Normal = 1,     // normal style
        Pressed,        // pressed style
        Dimmed,         // dimmed (disabled) style
        Highlighted,    // highlighted/focused style
        Expanded,       // expanded style
        LastStyleType   // leave this as last, also means style is undefined or invalid
    };

    Style(QObject *parent = 0);
    ~Style();

    void updateStyle();

    QString name() const;
    void setName(const QString &name);
    Set setType() const;
    void setSetType(Set set);
    
signals:

    void styleChanged();
    void nameChanged();
    void setChanged();
    
public slots:
    
private:
    QString m_name;
    Set m_set;
};

QML_DECLARE_TYPE(Style)
QML_DECLARE_TYPE(Style::Set)

#endif // STYLE_H
