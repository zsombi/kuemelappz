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
    int controlId;
    QDeclarativeItem *focusableControl;
    FocusControl *lastFocusItem;

    QList<ControlAction*> actions;
    FocusControl *prev;
    FocusControl *next;
    FocusControl *first;
    FocusControl *last;
    FocusControl *parent;
    CheckGroup *group;

    void _q_updateParent();
    void _q_handleFocusChange(bool f);
    void _q_handleChildrenChange();

    void updateParentSibling();
    void updateFocusSiblings();

/*
    FocusControl* prevFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* nextFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* firstFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* lastFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
*/

    void focusGroupElement();
    bool testAction(ControlAction::ActionFlags flags);
};

#endif // FOCUSCONTROL_P_H
