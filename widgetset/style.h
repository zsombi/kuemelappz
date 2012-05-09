#ifndef STYLE_H
#define STYLE_H

#include <QObject>
#include <qdeclarative.h>

class Style : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(StyleType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QVariantList types READ types WRITE setTypes NOTIFY typesChanged)

    Q_ENUMS(StyleType)
public:
    enum StyleType {
        Normal = 1,     // normal style
        Pressed,        // pressed style
        Dimmed,         // dimmed (disabled) style
        Highlighted,    // highlighted/focused style
        Expanded,       // expanded style
        LastStyleType   // leave this as last, also means style is undefined or invalid
    };

    Style(QObject *parent = 0);
    ~Style();

    QString name() const;
    void setName(const QString &name);
    StyleType type() const;
    void setType(StyleType type);
    QVariantList types() const;
    void setTypes(const QVariantList &t);
    
signals:

    void styleChanged();
    void nameChanged();
    void typeChanged();
    void typesChanged();
    
public slots:
    
private:
    QString m_name;
    StyleType m_type;
    QVariantList m_types;
};

QML_DECLARE_TYPE(Style)
QML_DECLARE_TYPE(Style::StyleType)

#endif // STYLE_H
