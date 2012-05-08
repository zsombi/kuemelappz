/**
  Screen - provides information about the current screen, serves for resolution and
  DPI calculation.

  */
#include "screen.h"
#include "globaldefs.h"

#include <QDesktopWidget>
#include <QDesktopServices>
#include <QApplication>

#define TRACE_SCREEN

Q_GLOBAL_STATIC(Screen, screenInst)

class ScreenPrivate
{
    Q_DECLARE_PUBLIC(Screen)
public:
    ScreenPrivate(Screen *qq);
    ~ScreenPrivate(){}

    Screen *q_ptr;
    QDesktopWidget *desktop;
    int appScreen;
    int screenCount;

    void _q_updateScreenCount(int count);
    void _q_updateScreenSize(int screen);
};

ScreenPrivate::ScreenPrivate(Screen *qq) :
    q_ptr(qq),
    desktop(QApplication::desktop())
{
    appScreen = desktop->primaryScreen();
    screenCount = desktop->screenCount();

    QObject::connect(desktop, SIGNAL(screenCountChanged(int)), qq, SLOT(_q_updateScreenCount(int)));
}

void ScreenPrivate::_q_updateScreenCount(int count)
{
    screenCount = count;
}
void ScreenPrivate::_q_updateScreenSize(int screen)
{
    Q_Q(Screen);
    emit q->displayChanged();
}

/*************************************************************************
 *************************************************************************/
Screen::Screen(QObject *parent) :
    QObject(parent),
    d_ptr(new ScreenPrivate(this))
{
}
Screen::~Screen()
{}

Screen *Screen::instance()
{
    return screenInst();
}

int Screen::width() const
{
    Q_D(const Screen);
#ifdef TRACE_SCREEN
    qDebug() << "Screen width=" << d->desktop->screenGeometry(d->appScreen).width();
#endif
    return d->desktop->screenGeometry(d->appScreen).width();
}
int Screen::height() const
{
    Q_D(const Screen);
#ifdef TRACE_SCREEN
    qDebug() << "Screen height=" << d->desktop->screenGeometry(d->appScreen).height();
#endif
    return d->desktop->screenGeometry(d->appScreen).height();
}
qreal Screen::dpi() const
{
    Q_D(const Screen);
    return d->desktop->physicalDpiX();
}

#include "moc_screen.cpp"
