#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <QtDeclarative/QDeclarativeItem>

class ScreenPrivate;
class Screen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width NOTIFY displayChanged)
    Q_PROPERTY(int height READ height NOTIFY displayChanged)
    Q_PROPERTY(qreal dpi READ dpi NOTIFY displayChanged)
    Q_PROPERTY(Orientation orientation READ orientation NOTIFY orientationChanged FINAL)
    Q_PROPERTY(QString orientationString READ orientationString NOTIFY orientationChanged FINAL)
    Q_PROPERTY(Orientation lockOnOrientation READ lockOnOrientation WRITE setLockOnOrientation NOTIFY orientationChanged FINAL)
    Q_PROPERTY(qreal rotation READ rotationAngle NOTIFY orientationChanged FINAL)
    Q_PROPERTY(DisplayType displayType READ displayType CONSTANT FINAL)
    Q_PROPERTY(QString typeString READ typeString CONSTANT FINAL)
    Q_PROPERTY(DisplayDensity displayDensity READ displayDensity CONSTANT FINAL)
    Q_PROPERTY(QString densityString READ densityString CONSTANT FINAL)

    Q_ENUMS(Orientation DisplayType DisplayDensity)
public:
    enum Orientation {
        Automatic,
        Portrait,
        Landscape,
        InverseLandscape
    };
    enum DisplayType {
        Small,
        Normal,
        Large,
        Desktop
    };

    enum DisplayDensity {
        Low,
        Medium,
        High,
        ExtraHigh
    };

    Screen(QObject *parent = 0);
    ~Screen();

    static Screen *instance();

    int width() const;
    int height() const;
    qreal dpi() const;
    Orientation orientation() const;
    QString orientationString() const;
    Orientation lockOnOrientation() const;
    void setLockOnOrientation(Orientation o);
    qreal rotationAngle() const;
    DisplayType displayType() const;
    QString typeString() const;
    DisplayDensity displayDensity() const;
    QString densityString() const;

Q_SIGNALS:
    void displayChanged();
    void displayTypeChanged();
    void orientationChangeStarted();
    void orientationChanged();
    void orientationChangeEnded();

public Q_SLOTS:
    
private:
    Q_DISABLE_COPY(Screen)
    Q_DECLARE_PRIVATE(Screen)
    QScopedPointer<ScreenPrivate> d_ptr;
    Q_PRIVATE_SLOT(d_func(), void _q_updateScreenCount(int count))
    Q_PRIVATE_SLOT(d_func(), void _q_updateScreenSize(int screen))
    Q_PRIVATE_SLOT(d_func(), void _q_updateSensorData())

};

QML_DECLARE_TYPE(Screen)
QML_DECLARE_TYPE(Screen::Orientation)
QML_DECLARE_TYPE(Screen::DisplayType)
QML_DECLARE_TYPE(Screen::DisplayDensity)

#endif // SCREEN_H
