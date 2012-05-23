#ifndef StyledItem_P_H
#define StyledItem_P_H

#include "styleditem.h"
#include "focuscontrol_p.h"

class Style;
class Theme;
class CheckGroup;

class StyledItemPrivate
{
    Q_DECLARE_PUBLIC(StyledItem)

public:

    StyledItemPrivate(StyledItem *qq);
    ~StyledItemPrivate();

    StyledItem *q_ptr;

    QString styleName;
    Style::StyleType currentStyle;
    QList<Style*> localStyles;

    Style* styleForType(Style::StyleType type) const;

    void _q_activateStyle();
    void _q_updateKeyOrder();
};

#endif // StyledItem_P_H
