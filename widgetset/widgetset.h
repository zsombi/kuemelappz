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
    Q_PROPERTY(QDeclarativeItem *inputPanel READ inputPanel NOTIFY inputPanelChanged)
    Q_ENUMS(FadeStyles PageStatus)
public:

    enum FadeStyles {
        FadeInOut,
        FadeFromLeft,
        FadeFromTop,
        FadeFromRight,
        FadeFromBottom,
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

    Q_INVOKABLE QDeclarativeItem *rootBodyItem();
    QDeclarativeItem *inputPanel();

public:
    bool mobilePlatform();

signals:
    void inputPanelChanged();
    void mobilePlatformChanged();
    
public slots:

public:
    QDeclarativeItem *appWindow;
};

QML_DECLARE_TYPE(WidgetSet)
QML_DECLARE_TYPE(WidgetSet::FadeStyles)
QML_DECLARE_TYPE(WidgetSet::PageStatus)

#endif // WIDGETSET_H
