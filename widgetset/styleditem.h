#ifndef StyledItem_H
#define StyledItem_H

#include "focuscontrol.h"
#include "style.h"


class StyledItemPrivate;
class StyledItem : public FocusControl
{
    Q_OBJECT
    Q_DISABLE_COPY(StyledItem)
    // style properties
    Q_PROPERTY(QString styleName READ styleName WRITE setStyleName NOTIFY styleNameChanged)
    Q_PROPERTY(Style::StyleType styleType READ styleType WRITE setStyleType NOTIFY styleChanged FINAL)
    Q_PROPERTY(Style* style READ style NOTIFY styleChanged FINAL)
    Q_PROPERTY(QDeclarativeListProperty<Style> localStyles READ localStyles)

public:
    StyledItem(QDeclarativeItem *parent = 0);
    ~StyledItem();

public slots:

signals:
    void styleChanged();
    void styleNameChanged();

protected: // properties

    Style *style() const;
    Style::StyleType styleType() const;
    void setStyleType(Style::StyleType type);
    QDeclarativeListProperty<Style> localStyles();
    QString styleName() const;
    void setStyleName(const QString &val);

private:
    Q_DECLARE_PRIVATE(StyledItem)
    QScopedPointer<StyledItemPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_updateKeyOrder())
    Q_PRIVATE_SLOT(d_func(), void _q_activateStyle())
};

QML_DECLARE_TYPE(StyledItem)

#endif // StyledItem_H

