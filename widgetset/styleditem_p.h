#ifndef StyledItem_P_H
#define StyledItem_P_H

#include "styleditem.h"

class Style;
class Theme;

class StyledItemPrivate
{
    Q_DECLARE_PUBLIC(StyledItem)

public:

    StyledItemPrivate(StyledItem *qq);
    ~StyledItemPrivate();

    StyledItem *q_ptr;

    Theme *activeTheme;
    QString styleName;
    Style::Set currentStyle;
    QDeclarativeItem *controlListItem;
    QList<Style*> localStyles;

    void reloadStyle();
    Style* styleForSet(Style::Set set) const;

    void _q_activateTheme();
    void _q_activateStyle();
    void _q_updateParent();
    void _q_handleFocusChange(bool f);
    void _q_updateKeyOrder();
};

#endif // StyledItem_P_H
