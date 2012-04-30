#ifndef CHECKGROUP_P_H
#define CHECKGROUP_P_H

#include "checkgroup.h"

class CheckGroupPrivate
{
    Q_DECLARE_PUBLIC(CheckGroup)
public:
    CheckGroupPrivate(CheckGroup *qq);
    void _q_uncheckPrev(int checkId);
    void _q_updateParent();
    void _q_updateGroupItems();
    void calculateItemSize();
    void handleRadio(QObject *item);

    void addItem(QObject *item, int checkId);
    void delItem(QObject *item);

public:
    CheckGroup *q_ptr;

    QSignalMapper *signalMap;
    QObject *prevItem;
    QObject *activeItem;
    QDeclarativeItem *layout;
    Qt::Orientation orientation;
    int activeItemId;
    bool m_autoId;
    int itemSize;
    int itemSpacing;
    bool bodyFit;
    int lastId;
};



#endif // CHECKGROUP_P_H
