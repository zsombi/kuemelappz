#ifndef FOCUSCONTROL_P_H
#define FOCUSCONTROL_P_H

#include "focuscontrol.h"

class CheckGroup;
class FocusControlPrivate
{
    Q_DECLARE_PUBLIC(FocusControl)
public:
    FocusControlPrivate(FocusControl* qq);
    virtual ~FocusControlPrivate();

    FocusControl *q_ptr;
    FocusControl::FocusType focusType;
    CheckGroup *group;
    int controlId;
    QDeclarativeItem *focusableControl;
    FocusControl *lastFocusItem;
    QList<DeclarativeAction*> actions;

    void _q_updateParent();
    void _q_handleFocusChange(bool f);

    FocusControl* prevFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* nextFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* firstFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* lastFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);

    FocusControl* firstFocusableChild();

    void focusGroupElement();
};

#endif // FOCUSCONTROL_P_H
