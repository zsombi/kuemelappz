#ifndef StyledItem_H
#define StyledItem_H

#include <QtDeclarative/QDeclarativeItem>
#include "style.h"

class StyledItemPrivate;
class StyledItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(StyledItem)
    // style properties
    Q_PROPERTY(QString styleName READ styleName WRITE setStyleName NOTIFY styleNameChanged)
    Q_PROPERTY(Style::StyleType styleType READ styleType WRITE setStyleType NOTIFY styleChanged FINAL)
    Q_PROPERTY(Style* style READ style NOTIFY styleChanged FINAL)
    Q_PROPERTY(QDeclarativeListProperty<Style> localStyles READ localStyles)
    // controlListItem - layout element holding StyledItem derivates; for key focus order handling
    Q_PROPERTY(QDeclarativeItem* controlListItem READ controlListItem WRITE setControlListItem NOTIFY controlListItemChanged)

public:
    StyledItem(QDeclarativeItem *parent = 0);
    ~StyledItem();

signals:
    void styleChanged();
    void styleNameChanged();
    void controlListItemChanged();

protected: // properties
    void keyPressEvent(QKeyEvent *event);

    Style *style() const;
    Style::StyleType styleType() const;
    void setStyleType(Style::StyleType type);
    QDeclarativeListProperty<Style> localStyles();
    QString styleName() const;
    void setStyleName(const QString &val);

    QDeclarativeItem* controlListItem() const;
    void setControlListItem(QDeclarativeItem *list);

private:
    Q_DECLARE_PRIVATE(StyledItem)
    QScopedPointer<StyledItemPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_handleFocusChange(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_updateKeyOrder())
    Q_PRIVATE_SLOT(d_func(), void _q_activateStyle())
    Q_PRIVATE_SLOT(d_func(), void _q_updateParent())
};

QML_DECLARE_TYPE(StyledItem)

#endif // StyledItem_H

