#ifndef STYLE_H
#define STYLE_H

#include <QObject>
#include <qdeclarative.h>

class Style : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(StyleTypes type READ type WRITE setType NOTIFY typeChanged)

    Q_FLAGS(StyleType StyleTypes)
public:
    enum StyleType {
        Undefined = 0,      // style is not defined
        Normal = 0x01,      // normal style
        Pressed = 0x02,     // pressed style
        Dimmed = 0x04,      // dimmed (disabled) style
        Highlighted = 0x08, // highlighted/focused style
        Expanded = 0x10     // expanded style (deprecate?)
    };
    Q_DECLARE_FLAGS(StyleTypes, StyleType)

    Style(QObject *parent = 0);
    ~Style();

    QString name() const;
    void setName(const QString &name);
    StyleTypes type() const;
    void setType(StyleTypes type);
    
signals:

    void styleChanged();
    void nameChanged();
    void typeChanged();
    
public slots:
    
private:
    QString m_name;
    StyleTypes m_type;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Style::StyleTypes)

QML_DECLARE_TYPE(Style)

#endif // STYLE_H
