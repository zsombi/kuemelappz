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
public:
    Screen(QObject *parent = 0);
    ~Screen();

    static Screen *instance();

    int width() const;
    int height() const;
    qreal dpi() const;
    
Q_SIGNALS:
    void displayChanged();
    
public Q_SLOTS:
    
private:
    Q_DISABLE_COPY(Screen)
    Q_DECLARE_PRIVATE(Screen)
    QScopedPointer<ScreenPrivate> d_ptr;
    Q_PRIVATE_SLOT(d_func(), void _q_updateScreenCount(int count))
    Q_PRIVATE_SLOT(d_func(), void _q_updateScreenSize(int screen))

};

QML_DECLARE_TYPE(Screen)

#endif // SCREEN_H
