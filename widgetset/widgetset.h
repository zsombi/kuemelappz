#ifndef WIDGETSET_H
#define WIDGETSET_H

#include <QObject>
#include <QtDeclarative/QDeclarativeItem>

/*
  Utility class, defining few flags, properties to differentiate WidgetSet behavior in desktop/mobile environment.
  */

class QDeclarativeItem;

class WidgetSet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool mobilePlatform READ mobilePlatform NOTIFY mobilePlatformChanged)
    Q_PROPERTY(QDeclarativeItem *applicationBody READ rootBodyItem NOTIFY applicationBodyChanged)
    Q_PROPERTY(QDeclarativeItem *statusBar READ statusBar NOTIFY statusBarChanged)
    Q_PROPERTY(QDeclarativeItem *inputPanel READ inputPanel NOTIFY inputPanelChanged)
    Q_PROPERTY(QDeclarativeItem *headerPanel READ headerPanel NOTIFY headerPanelChanged)
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_ENUMS(FaderStyle PageStatus)
public:

    enum FaderStyle {
        FadeInOut,
        FadeLeft,
        FadeTop,
        FadeRight,
        FadeBottom,
        FadeInsert
    };

    enum PageStatus {
        PageInactive,
        PageActivating,
        PageActive,
        PageDeactivating
    };


    WidgetSet(QObject *parent = 0);
    ~WidgetSet();

    static void initialize(QDeclarativeContext *context);
    static WidgetSet *instance();

    QDeclarativeItem *rootBodyItem();
    QDeclarativeItem *statusBar();
    QDeclarativeItem *inputPanel();
    QDeclarativeItem *headerPanel();
    bool busy() const;
    void setBusy(bool v);

public:
    bool mobilePlatform();

signals:
    void mobilePlatformChanged();
    void busyChanged();
    // these signals are needed for proper property binding...
    void applicationBodyChanged();
    void statusBarChanged();
    void inputPanelChanged();
    void headerPanelChanged();
    
public slots:

public:
    QDeclarativeItem *appWindow;
    int busyCount;
};

QML_DECLARE_TYPE(WidgetSet)
QML_DECLARE_TYPE(WidgetSet::FaderStyle)
QML_DECLARE_TYPE(WidgetSet::PageStatus)

#endif // WIDGETSET_H
