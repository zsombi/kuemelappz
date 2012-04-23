#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

#include <QDebug>
#include <qplatformdefs.h> // MEEGO_EDITION_HARMATTAN

#if defined Q_SIMULATOR && !defined(QT_BOOTSTRAPPED)
#define Q_OS_SIMULATOR
#endif

#if defined(Q_OS_SYMBIAN) || defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_SIMULATOR)
#define MOBILE_SYSTEM
#endif


#endif // GLOBALDEFS_H
