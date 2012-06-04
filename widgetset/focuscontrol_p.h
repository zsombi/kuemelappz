#ifndef FOCUSCONTROL_P_H
#define FOCUSCONTROL_P_H

#include "focuscontrol.h"

class CheckGroup;
class FocusControlPrivate
{
    Q_DECLARE_PUBLIC(FocusControl)
public:
    enum SiblingType {
        Previous = 1,
        Next,
        First,
        Last,
        Parent,
        Root,
        FocusItem
    };

    FocusControlPrivate(FocusControl* qq);
    virtual ~FocusControlPrivate();

    FocusControl *q_ptr;
    FocusControl::FocusType focusType;
    int controlId;
    QDeclarativeItem *focusableControl;
    FocusControl *lastFocusItem;
    FocusControl *parentSibling; // need to know where the control was regsitered first

    // need a list to hold focus control instances as groups may not be accessible as siblings
    QList<FocusControl*> controlList;

    QList<ControlAction*> actions;
    CheckGroup *group;

    void _q_updateParent();
    void _q_handleFocusChange(bool f);
    void _q_handleChildrenChange();

    void addControlToParent();
    FocusControl *focusSibling(SiblingType siblingType, FocusControl *relativeTo = 0);

/*
    FocusControl* prevFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* nextFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* firstFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
    FocusControl* lastFocusable(FocusControl *rel = 0, FocusControl::FocusType focus = FocusControl::Focusable);
*/

    //void focusGroupElement();
    bool testAction(ControlAction::ActionFlags flags);
};

#endif // FOCUSCONTROL_P_H
