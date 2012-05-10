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

    Q_ENUMS(Orientation)
public:
    enum Orientation {
        Automatic,
        Portrait,
        Landscape,
        InverseLandscape
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

Q_SIGNALS:
    void displayChanged();
    void orientationChanged();
    
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

#endif // SCREEN_H
