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
    Q_PROPERTY(bool mobilePlatform READ mobilePlatform CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeItem *applicationBody READ rootBodyItem CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeItem *statusBar READ statusBar CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeItem *toolBar READ toolBar CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeItem *inputPanel READ inputPanel CONSTANT FINAL)
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_ENUMS(FaderStyle PageStatus)
    Q_FLAGS(InputLayout InputLayoutFlags)
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

    enum InputLayout {
        InputWithClose = 0x01,
        InputWithNavigators = 0x02,
        InputWithEditor = 0x04,
        InputWithClear = 0x08
    };
    Q_DECLARE_FLAGS(InputLayoutFlags, InputLayout)

    WidgetSet(QObject *parent = 0);
    ~WidgetSet();

    static void initialize(QDeclarativeContext *context);
    static WidgetSet *instance();

    QDeclarativeItem *rootBodyItem();
    QDeclarativeItem *statusBar();
    QDeclarativeItem *toolBar();
    QDeclarativeItem *inputPanel();
    bool busy() const;
    void setBusy(bool v);

public:
    bool mobilePlatform();

signals:
    void busyChanged();
    
public slots:

public:
    QDeclarativeItem *appWindow;
    int busyCount;
};

QML_DECLARE_TYPE(WidgetSet)
QML_DECLARE_TYPE(WidgetSet::FaderStyle)
QML_DECLARE_TYPE(WidgetSet::PageStatus)
QML_DECLARE_TYPE(WidgetSet::InputLayout)

#endif // WIDGETSET_H
